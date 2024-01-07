/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** main
*/

#include "Server.hpp"
#include "KeyEventMapping.hpp"

std::pair<int, int> Server::get_position_change_for_event(entity_t entity, int event)
{
    auto &animatedDrawable = _ecs.get_components<component::AnimatedDrawable>()[entity];
    std::string prevState = animatedDrawable.value()._state;
    if (event == KeyIds["Up"]) {
        // animatedDrawable.value()._state = "move up";
        // send_animated_drawable_update_to_all_clients(entity, animatedDrawable.value()._state);
        return {0, -30};
    }
    if (event == KeyIds["Down"]) {
        // animatedDrawable.value()._state = "move down";
        // send_animated_drawable_update_to_all_clients(entity, animatedDrawable.value()._state);
        return {0, 30};
    }
    // animatedDrawable.value()._state = "idle";
    if (prevState != animatedDrawable.value()._state)
        send_animated_drawable_update_to_all_clients(entity, animatedDrawable.value()._state);
    if (event == KeyIds["Left"])
        return {-30, 0};
    if (event == KeyIds["Right"])
        return {30, 0};
    if (event == KeyIds["Space"])
        _listener.addEvent(new ShootEvent(entity, -1));
    return {0, 0};
}

Server::Server(asio::io_context& service, int port, registry& ecs, EventListener& listener)
    : _service(service),
      _socket(service, udp::endpoint(udp::v4(), port)),
      _ecs(ecs),
      _listener(listener),
      _send_thread(&Server::sendPositionpacketsPeriodically, this)
{
    _tpool.emplace_back([this, &service]() {
        service.run();
    });
    try {
        connectToDB();
        // signUp("Jacques", "Jacques");
        // signIn("admin", "admin");
        // addHighScore("admin", 100);
        // addHighScore("Anatole", 110);
        // addHighScore("Pierre", 80);
        // addHighScore("Jacques", 90);
        // getHighScore();
        // addFriend("admin", "9AEPR4G1XK");
        // addFriend("Anatole", "Pierre");
        // addFriend("Anatole", "admin");
        // addFriend("Jacques", "tests");
        // removeFriend("admin", "9AEPR4G1XK");
        // displayFriends("admin");
    } catch (const std::exception& e) {
        std::cout<< "Exception: " << e.what() << std::endl;
    }
    _tpool.emplace_back([this, &service]() { service.run(); });
    recieve_from_client();
}

entity_t Server::get_player_entity_from_connection_address(udp::endpoint endpoint)
{
    sparse_array<component::Endpoint> all_endpoints = _ecs.get_components<component::Endpoint>();

    for (size_t i = 0; i < all_endpoints.size(); ++i) {
        if (all_endpoints[i].has_value() && all_endpoints[i].value()._endpoint == endpoint)
            return i;
    }

    return -1;
}

entity_t Server::connect_player(udp::endpoint player_endpoint)
{
    std::cout << "Connection" << std::endl;
    entity_t new_player = _ecs.spawn_entity();
    _ecs.mtx.lock();
    can_send = false;
    _ecs.add_component(new_player, component::Position(10.0f, 10.0f));
    can_send = true;
    _ecs.add_component(new_player, component::ResetOnMove());
    _ecs.add_component(new_player, component::Controllable());
    _ecs.add_component(new_player, component::Heading());
    _ecs.add_component(new_player, component::AnimatedDrawable("temp/assets/textures/sprites/r-typesheet42.gif", {5, 1}, {32, 14}, {1, 0}, {1, 20}, {0, 0}));
    _ecs.add_component(new_player, component::Hitbox(component::Position(32 * 8.5, 14 * 8.5)));
    auto &tmp = _ecs.get_components<component::AnimatedDrawable>()[new_player];
    tmp->addAnimation("idle", {2, 2}, false);
    tmp->addAnimation("move up", {2, 4}, false);
    tmp->addAnimation("move down", {2, 0}, false);
    tmp->_state = "idle";
    _ecs.add_component(new_player, component::Endpoint(player_endpoint));
    _ecs.add_component(new_player, component::Scale(8.5f));
    _ecs.add_component(new_player, component::Rotation(90));
    _ecs.add_component(new_player, component::Health(100));
    _ecs.add_component(new_player, component::Damage(20));
    _ecs.add_component(new_player, component::Score());

    entity_t enemy = _ecs.spawn_entity();

    can_send = false;
    _ecs.add_component<component::Position>(enemy, component::Position(1600, 500));
    can_send = true;
    _ecs.add_component<component::Velocity>(enemy, component::Velocity(-8.0f, 0.0f));
    _ecs.add_component<component::Scale>(enemy, component::Scale(6.0f));
    _ecs.add_component<component::AnimatedDrawable>(enemy, component::AnimatedDrawable("temp/assets/textures/sprites/r-typesheet5.gif", {7, 0}, {21, 24}, {12, 0}, {5, 5}));

    auto &tmp1 = _ecs.get_components<component::AnimatedDrawable>()[enemy];
    tmp1->addAnimation("idle", {0, 7}, true);
    tmp1->_state = "idle";

    std::cout << "New player connected !" << std::endl;
    send_animated_drawable_snapshots_for_specific_player(new_player);
    send_animated_drawable_snapshot_to_all_players(new_player);
    send_all_entity_drawables_to_specific_player(new_player);
    send_highscore_to_specific_client(new_player);
    send_all_scale_to_player(new_player);
    send_scale_to_all_players(new_player);
    _ecs.mtx.unlock();
    return new_player;
}

void Server::send_all_scale_to_player(entity_t entity)
{
    auto scale = _ecs.get_components<component::Scale>();

    for (int i = 0; i < scale.size(); ++i) {
        if (!scale[i].has_value())
            continue;
        ScaleSnapshot to_send(15, i, *scale[i], _packet_id);
        _packet_id++;
        _scale_packets.push_back(to_send);
        send_data_to_client_by_entity(to_send, entity);
    }
}

void Server::send_scale_to_all_players(entity_t entity)
{
    auto scale = _ecs.get_components<component::Scale>()[entity];

    if (!scale.has_value())
        return;
    ScaleSnapshot to_send(15, entity, *scale, 0);
    send_data_to_all_clients(to_send, _scale_packets);
}

void Server::send_all_entity_drawables_to_specific_player(entity_t player)
{
    auto drawables = _ecs.get_components<component::Drawable>();

    for (int i = 0; i < drawables.size(); ++i) {
        if (!drawables[i].has_value())
            continue;
        DrawableSnapshot to_send(6, player, drawables[i].value()._path, _packet_id);
        _packet_id += 1;
        _drawable_packets.push_back(to_send);
        send_data_to_client_by_entity(to_send, player);
    }
}

void Server::send_highscore_to_specific_client(entity_t new_player)
{
    HighScoreMessage highscoreMsg = getHighScore();
    send_data_to_client_by_entity<HighScoreMessage>(highscoreMsg, new_player);
}

std::vector<char> Server::recieve_raw_data_from_client()
{
    std::vector<char> receivedData(MAX_BUF_SIZE);
    size_t bytesRead = _socket.receive_from(asio::buffer(receivedData), _remote_endpoint);

    receivedData.resize(bytesRead);
    return receivedData;
}

void Server::send_position_snapshots_for_all_players()
{
    while (!can_send) continue;
    while (!_ecs.can_run_updates) continue;
    // std::cout << "SENDING TO ALL PLAYERS\n";
    _ecs.can_run_updates = false;
    _ecs.mtx.lock();
    std::vector<SnapshotPosition> to_send = {};
    sparse_array<component::Position> &pos = _ecs.get_components<component::Position>();
    for (size_t i = 0; i < pos.size(); i++) {
        if (pos[i].has_value()) {
            // std::cout << "position: x "  << pos[i].value().x << ", y " << pos[i].value().y << std::endl;
            to_send.push_back(SnapshotPosition(4, i, component::Position(pos[i].value().x, pos[i].value().y), 0));

        }
    }
    for (SnapshotPosition &pos: to_send) {
        send_data_to_all_clients<SnapshotPosition>(pos, _position_packets);
    }
    _ecs.mtx.unlock();
    _ecs.can_run_updates = true;
    // std::cout << "DONE SENDING TO ALL PLAYERS\n";
}

void Server::send_animated_drawable_update_to_all_clients(entity_t entity, std::string state)
{
    if (state.size() > 15) {
        std::cout << "ERROR STATE SIZE TOO BIG TO BE STORED\n";
        return;
    }
    // std::cout << "sending update\n";
    AnimatedStateUpdateMessage to_send(14, entity, state, 0);
    send_data_to_all_clients(to_send, _animated_drawable_update_packets);
}

void Server::send_animated_drawable_snapshot_to_all_players(entity_t entity)
{
    auto &animatedDrawable = _ecs.get_components<component::AnimatedDrawable>()[entity];
    if (animatedDrawable.has_value()) {
        AnimatedDrawableSnapshot snap_ad(
            13,
            entity,
            animatedDrawable.value()._path,
            animatedDrawable.value()._nbSprites,
            animatedDrawable.value()._spriteSize,
            animatedDrawable.value()._gaps,
            animatedDrawable.value()._firstOffset,
            animatedDrawable.value()._currentIdx,
            animatedDrawable.value()._anims,
            animatedDrawable.value()._state,
            0
        );
        while (!can_send) continue;
        send_data_to_all_clients(snap_ad, _animated_drawable_packets);
    }
}

void Server::send_animated_drawable_snapshots_for_specific_player(entity_t entity)
{
    sparse_array<component::AnimatedDrawable> animatedDrawables = _ecs.get_components<component::AnimatedDrawable>();
    for (size_t i = 0; i < animatedDrawables.size(); i++) {
        if (animatedDrawables[i].has_value()) {
            AnimatedDrawableSnapshot snap_ad(
                13,
                i,
                animatedDrawables[i].value()._path,
                animatedDrawables[i].value()._nbSprites,
                animatedDrawables[i].value()._spriteSize,
                animatedDrawables[i].value()._gaps,
                animatedDrawables[i].value()._firstOffset,
                animatedDrawables[i].value()._currentIdx,
                animatedDrawables[i].value()._anims,
                animatedDrawables[i].value()._state,
                _packet_id
            );
            _packet_id++;
            _animated_drawable_packets.push_back(snap_ad);
            send_data_to_client_by_entity(snap_ad, entity);
        }
    }
}

void Server::send_entity_drawable_to_all_players(entity_t entity)
{
    sparse_array<component::Drawable> drawables = _ecs.get_components<component::Drawable>();
    component::Drawable drawable = drawables[entity].value();
    DrawableSnapshot to_send(6, entity, drawable._path, 0);
    send_data_to_all_clients<DrawableSnapshot>(to_send, _drawable_packets);
}

void Server::send_death_event_to_all_players(entity_t entity)
{
    DeathEventMessage evt(16, entity, 0);
    send_data_to_all_clients(evt, _death_packets);
}

void Server::recieve_from_client()
{
    std::vector<char> client_msg = recieve_raw_data_from_client();
    // std::cout << "STARTING RECIEVING\n";
    if (client_msg.size() < sizeof(BaseMessage))
        recieve_from_client();
    entity_t player_entity = get_player_entity_from_connection_address(_remote_endpoint);
    BaseMessage *baseMsg = reinterpret_cast<BaseMessage *>(client_msg.data());

    if (player_entity == -1) {
        player_entity = connect_player(_remote_endpoint);
    }
    // std::cout << "message id: " << baseMsg->id << std::endl;
    if (_messageParser.find(baseMsg->id) == _messageParser.end())
        throw ArgumentError("ERROR: Invalid event recieved: " + std::to_string(baseMsg->id) + ".");
    (this->*_messageParser[baseMsg->id])(client_msg, player_entity);
    // std::cout << "FINISHED RECIEVING\n";
    recieve_from_client();
}

int Server::recieve_packet_confirm(std::vector<char> & client_msg, entity_t _) {
    ConfirmationMessage *confirmMsg = reinterpret_cast<ConfirmationMessage *>(client_msg.data());
    int id = confirmMsg->packet_id;

    while (!can_mod) continue;
    _position_packets.erase(
        std::remove_if(_position_packets.begin(), _position_packets.end(), [id](const SnapshotPosition& snapshot) {
            return snapshot.packet_id == id;
        }
        ),
        _position_packets.end()
    );
    _drawable_packets.erase(
        std::remove_if(_drawable_packets.begin(), _drawable_packets.end(), [id](const DrawableSnapshot& snapshot) {
            return snapshot.packet_id == id;
        }
        ),
        _drawable_packets.end()
    );
    _highscore_packets.erase(
        std::remove_if(_highscore_packets.begin(), _highscore_packets.end(), [id](const HighScoreMessage& snapshot) {
            return snapshot.packet_id == id;
        }
        ),
        _highscore_packets.end()
    );
    _animated_drawable_packets.erase(
        std::remove_if(_animated_drawable_packets.begin(), _animated_drawable_packets.end(), [id](const AnimatedDrawableSnapshot& snapshot) {
            return snapshot.packet_id == id;
        }
        ),
        _animated_drawable_packets.end()
    );
    _animated_drawable_update_packets.erase(
        std::remove_if(_animated_drawable_update_packets.begin(), _animated_drawable_update_packets.end(), [id](const AnimatedStateUpdateMessage& snapshot) {
            return snapshot.packet_id == id;
        }
        ),
        _animated_drawable_update_packets.end()
    );
    _chat_packets.erase(
        std::remove_if(_chat_packets.begin(), _chat_packets.end(), [id](const ChatMessage& snapshot) {
            return snapshot.packet_id == id;
        }
        ),
        _chat_packets.end()
    );
    _scale_packets.erase(
        std::remove_if(_scale_packets.begin(), _scale_packets.end(), [id](const ScaleSnapshot& snapshot) {
            return snapshot.packet_id == id;
        }
        ),
        _scale_packets.end()
    );
    _death_packets.erase(
        std::remove_if(_death_packets.begin(), _death_packets.end(), [id](const DeathEventMessage& snapshot) {
            return snapshot.packet_id == id;
        }
        ),
        _death_packets.end()
    );
    
    return 0;
}

int Server::recieve_client_event(std::vector<char> &client_msg, entity_t player_entity)
{
    if (client_msg.size() < sizeof(EventMessage))
        return -1;
    EventMessage *event = reinterpret_cast<EventMessage *>(client_msg.data());
    // std::cout << "New event recieved from: " << _remote_endpoint << std::endl;
    // std::cout << "event recieved: " << event->event << std::endl;
    while (!_ecs.can_run_updates) continue;
    _ecs.can_run_updates = false;
    std::pair<int, int> to_move = get_position_change_for_event(player_entity, event->event);
    if (to_move.first != 0 || to_move.second != 0) {
        _listener.addEvent(new UpdatePositionEvent(player_entity, get_position_change_for_event(player_entity, event->event)));
        _listener.addEvent(new PositionStayInWindowBounds(player_entity, {0, 1920, 0, 1080}));
    }
    _ecs.can_run_updates = true;
    return 0;
}

int Server::recieve_connection_event(std::vector<char> &client_msg, entity_t player_entity)
{
    static_cast<void>(client_msg);
    static_cast<void>(player_entity);
    return 0;
}

int Server::recieve_disconnection_event(std::vector<char> &client_msg, entity_t player_entity)
{
    _ecs.kill_entity(player_entity);
    return 0;
}

int Server::receive_login_event(std::vector<char> &client_msg, entity_t player_entity) {
    if (client_msg.size() < sizeof(LoginMessage))
        return -1;
    LoginMessage *snapshot = reinterpret_cast<LoginMessage *>(client_msg.data());
    while (!can_read)
        continue;
    bool response = true;
    if (snapshot->logintype == 0)
        response = signUp(snapshot->username, snapshot->password);
    else if (snapshot->logintype == 1)
        response = signIn(snapshot->username, snapshot->password);
    LoginResponse resp(8, response, snapshot->logintype, _packet_id);
    send_data_to_client_by_entity<LoginResponse>(resp, player_entity);
    _packet_id += 1;
    return 0;
}

int Server::receive_friend_event(std::vector<char> &client_msg, entity_t player_entity)
{
    if (client_msg.size() < sizeof(FriendsMessage)) {
        printf("uy\n");
        return -1;
    }
    FriendsMessage *snapshot = reinterpret_cast<FriendsMessage *>(client_msg.data());
    while (!can_read)
        continue;
    std::vector<Friendship> friends = displayFriends(snapshot->username, player_entity);
    _packet_id++;
    return 0;
}

int Server::receive_add_friend_event(std::vector<char>& client_msg, entity_t player_entity)
{
    if (client_msg.size() < sizeof(AddFriendsMessage))
        return -1;
    AddFriendsMessage *snapshot = reinterpret_cast<AddFriendsMessage *>(client_msg.data());
    while (!can_read)
        continue;
    bool response = addFriend(snapshot->username, snapshot->friendName);
    AddFriendsResponse resp(10, response, _packet_id);
    send_data_to_client_by_entity<AddFriendsResponse>(resp, player_entity);
    return 0;
}

int Server::receive_remove_friend_event(std::vector<char>& client_msg, entity_t player_entity)
{
    if (client_msg.size() < sizeof(RemoveFriendsMessage))
        return -1;
    RemoveFriendsMessage *snapshot = reinterpret_cast<RemoveFriendsMessage *>(client_msg.data());
    while (!can_read)
        continue;
    bool response = removeFriend(snapshot->username, snapshot->friendName);
    RemoveFriendsResponse resp(11, response, _packet_id);
    send_data_to_client_by_entity<RemoveFriendsResponse>(resp, player_entity);
    return 0;
}

int Server::receive_chat_event(std::vector<char>& client_msg, entity_t player_entity)
{
    if (client_msg.size() < sizeof(ChatMessage))
        return -1;
    ChatMessage *snapshot = reinterpret_cast<ChatMessage *>(client_msg.data());
    while (!can_read)
        continue;
    ChatMessage reponse(12, snapshot->name, snapshot->content, _packet_id);
    send_data_to_all_clients<ChatMessage>(reponse, _chat_packets);
    return 0;
}

Server::~Server() {
    if (_send_thread.joinable())
        _send_thread.join();
}

template <typename T>
void Server::send_data_to_all_clients(T& structure, std::vector<T>& packets_to_send) {
    // std::cout << "GONNA SEND TO ALL\n";
    while (!can_send) continue;
    can_send = false;
    sparse_array<component::Endpoint> all_endpoints = _ecs.get_components<component::Endpoint>();
    for (size_t i = 0; i < all_endpoints.size(); i++) {
        if (all_endpoints[i].has_value()) {
            while (!can_mod) continue;
            structure.packet_id = _packet_id;
            _packet_id += 1;
            packets_to_send.push_back(structure);
            _socket.send_to(asio::buffer(&structure, sizeof(structure)), all_endpoints[i].value()._endpoint);
        }
    }
    can_send = true;
    // std::cout << "FINISHED SENDING TO ALL\n";
}

template <typename T>
void Server::send_data_to_all_clients_except_me(T& structure) {
    while (!can_send) continue;
    can_send = false;
    sparse_array<component::Endpoint> all_endpoints = _ecs.get_components<component::Endpoint>();
    for (size_t i = 0; i < all_endpoints.size(); i++) {
        if (all_endpoints[i].has_value() && all_endpoints[i].value()._endpoint != _remote_endpoint) {
            while (!can_mod) continue;
            _socket.send_to(asio::buffer(&structure, sizeof(structure)), all_endpoints[i].value()._endpoint);
        }
    }
    can_send = true;
}

template <typename T>
void Server::resend_packets(std::vector<T> &packets) {
    for (auto& packet : packets) {
        sparse_array<component::Endpoint> all_endpoints = _ecs.get_components<component::Endpoint>();
        for (size_t i = 0; i < all_endpoints.size(); i++) {
            if (all_endpoints[i].has_value())
                _socket.send_to(asio::buffer(&packet, sizeof(packet)), all_endpoints[i].value()._endpoint);
        }
    }
}

void Server::sendPositionpacketsPeriodically() {
    int counter = 0;
    std::vector<entity_t> animatedDrawableRegistered = {};
    std::vector<entity_t> entitiesAlive = {};
    std::vector<entity_t> scalesRegistered = {};
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::lock_guard<std::mutex> lock(mtx);
        if (counter >= 10) {
            while (!can_send) continue;
            can_mod = false;
            // std::cout << "resending packets\n";
            resend_packets<SnapshotPosition>(_position_packets);
            resend_packets<DrawableSnapshot>(_drawable_packets);
            resend_packets<AnimatedDrawableSnapshot>(_animated_drawable_packets);
            resend_packets<AnimatedStateUpdateMessage>(_animated_drawable_update_packets);
            resend_packets<HighScoreMessage>(_highscore_packets);
            resend_packets<ScaleSnapshot>(_scale_packets);
            resend_packets<DeathEventMessage>(_death_packets);
            // std::cout << "finished resending packets\n";
            can_mod = true;
            counter = 0;
        }
        // std::cout << "STARTED LOOP 2nd THREAD\n";
        auto tmp = _ecs.get_components<component::AnimatedDrawable>();
        for (size_t i = 0; i < tmp.size(); ++i) {
            if (tmp[i].has_value() && std::find(animatedDrawableRegistered.begin(), animatedDrawableRegistered.end(), i) == animatedDrawableRegistered.end()) {
                auto &endpoint = _ecs.get_components<component::Endpoint>()[i];
                animatedDrawableRegistered.push_back(i);
                if (!endpoint.has_value())
                    send_animated_drawable_snapshot_to_all_players(i);
            }
        }
        auto tmp1 = _ecs.get_components<component::Scale>();
        for (size_t i = 0; i < tmp1.size(); ++i) {
            if (tmp1[i].has_value() && std::find(scalesRegistered.begin(), scalesRegistered.end(), i) == scalesRegistered.end()) {
                auto &endpoint = _ecs.get_components<component::Endpoint>()[i];
                scalesRegistered.push_back(i);
                if (!endpoint.has_value())
                    send_scale_to_all_players(i);
            }
        }
        auto tmp2 = _ecs.get_components<component::Position>();
        for (size_t i = 0; i < entitiesAlive.size(); ++i) {
            if (!tmp2[entitiesAlive[i]].has_value()) {
                send_death_event_to_all_players(entitiesAlive[i]);
                entity_t ent = entitiesAlive[i];
                entitiesAlive.erase(
                std::remove_if(entitiesAlive.begin(), entitiesAlive.end(), [ent](const entity_t& entity) {
                        return entity == ent;
                    }
                    ),
                    entitiesAlive.end()
                );
            }
        }
        for (size_t i = 0; i < tmp2.size(); ++i) {
            if (tmp2[i].has_value() && std::find(entitiesAlive.begin(), entitiesAlive.end(), i) == entitiesAlive.end()) {
                entitiesAlive.push_back(i);
            }
        }
        // std::cout << "FINISHED LOOP 2nd THREAD\n";
        send_position_snapshots_for_all_players();
        ++counter;
    }
}
