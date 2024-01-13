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
    std::string prevState = animatedDrawable->_state;
    if (event == KeyIds["Up"]) {
        animatedDrawable->_state = "move up";
        send_animated_drawable_update_to_all_clients(entity, animatedDrawable->_state, _ecs.get_components<component::Endpoint>());
        return {0, -30};
    }
    if (event == KeyIds["Down"]) {
        animatedDrawable->_state = "move down";
        send_animated_drawable_update_to_all_clients(entity, animatedDrawable->_state, _ecs.get_components<component::Endpoint>());
        return {0, 30};
    }
    animatedDrawable->_state = "idle";
    send_animated_drawable_update_to_all_clients(entity, animatedDrawable->_state, _ecs.get_components<component::Endpoint>());
    if (event == KeyIds["Left"])
        return {-30, 0};
    if (event == KeyIds["Right"])
        return {30, 0};
    if (event == KeyIds["Space"])
        _listener.addEvent(new ShootEvent(entity, -1));
    return {0, 0};
}

// void Server::recieveThread() {
//     asio::io_context& service = _service;
//     for (int i = 0; i < 4; ++i)
//         _tpool.emplace_back([this, &service]() { service.run(); });
//     recieve_from_client();
// }

void Server::operator()(sparse_array<component::AnimatedDrawable> &dra, sparse_array<component::Scale> &scl, sparse_array<component::Position> &pos, sparse_array<component::Endpoint> &edp) {
    if (resend_counter > 10) {
        resend_packets<SnapshotPosition>(_position_packets, edp);
        resend_packets<DrawableSnapshot>(_drawable_packets, edp);
        resend_packets<AnimatedDrawableSnapshot>(_animated_drawable_packets, edp);
        resend_packets<AnimatedStateUpdateMessage>(_animated_drawable_update_packets, edp);
        resend_packets<HighScoreMessage>(_highscore_packets, edp);
        resend_packets<ScaleSnapshot>(_scale_packets, edp);
        resend_packets<DeathEventMessage>(_death_packets, edp);
        resend_packets<RoomCreationMessage>(_room_creation_packets, edp);
        resend_packets<RoomJoinMessage>(_room_join_packets, edp);
        timer.restart();
        resend_counter = 0;
    }
    if (timer.getElapsedTime() > 100) {
        timer.restart();
        for (size_t i = 0; i < dra.size(); ++i) {
            if (dra[i].has_value() && std::find(animatedDrawableRegistered.begin(), animatedDrawableRegistered.end(), i) == animatedDrawableRegistered.end()) {
                auto &endpoint = _ecs.get_components<component::Endpoint>()[i];
                animatedDrawableRegistered.push_back(i);
                if (!endpoint.has_value()) {
                    send_animated_drawable_snapshot_to_all_players(i, dra, edp);
                }
            }
        }
        for (size_t i = 0; i < scl.size(); ++i) {
            if (scl[i].has_value() && std::find(scalesRegistered.begin(), scalesRegistered.end(), i) == scalesRegistered.end()) {
                auto &endpoint = edp[i];
                scalesRegistered.push_back(i);
                if (!endpoint.has_value()) {
                    send_scale_to_all_players(i, scl, edp);
                }
            }
        }
        for (size_t i = 0; i < entitiesAlive.size(); ++i) {
            if (!pos[entitiesAlive[i]].has_value()) {
                send_death_event_to_all_players(entitiesAlive[i], edp);
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
        for (size_t i = 0; i < pos.size(); ++i) {
            if (pos[i].has_value() && std::find(entitiesAlive.begin(), entitiesAlive.end(), i) == entitiesAlive.end()) {
                entitiesAlive.push_back(i);
            }
        }
        send_position_snapshots_for_all_players(pos, edp);
        send_health_to_specific_client(pos, edp);
        send_score_to_specific_client(pos, edp);
        ++resend_counter;
    }
    recieve_from_client();
};

Server::Server(asio::io_context& service, int port, registry& ecs, EventListener& listener, std::mutex &mtx_)
    : _service(service),
      _socket(service, udp::endpoint(udp::v4(), port)),
      _ecs(ecs),
      _listener(listener),
    //   _send_thread(&Server::recieveThread, this),
      mtx(mtx_)
{
    try {
        connectToDB();
    } catch (const std::exception& e) {
        std::cout<< "Exception: " << e.what() << std::endl;
    }
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

entity_t Server::connect_player(udp::endpoint player_endpoint, std::string username, std::string room_name)
{
    std::cout << "Connection" << std::endl;
    entity_t new_player = _ecs.spawn_entity();
    _ecs.add_component(new_player, component::Position(10.0f, 10.0f));
    _ecs.add_component(new_player, component::ResetOnMove());
    _ecs.add_component(new_player, component::Controllable());
    _ecs.add_component(new_player, component::Heading());
    _ecs.add_component(new_player, component::AnimatedDrawable("temp/assets/textures/sprites/r-typesheet42.gif", {5, 1}, {32, 14}, {1, 0}, {1, 20}));
    _ecs.add_component(new_player, component::Hitbox(component::Position(32 * 6.0f, 14 * 6.0)));
    auto &tmp = _ecs.get_components<component::AnimatedDrawable>()[new_player];
    tmp->addAnimation("idle", {2, 2}, false);
    tmp->addAnimation("move up", {2, 4}, false);
    tmp->addAnimation("move down", {2, 0}, false);
    tmp->_state = "idle";
    _ecs.add_component(new_player, component::Endpoint(player_endpoint));
    _ecs.add_component(new_player, component::Room(room_name));
    _ecs.add_component(new_player, component::Username(username));
    if (username == _lobbies[room_name])
        _ecs.add_component(new_player, component::Host());
    _ecs.add_component(new_player, component::Scale(6.0f));
    _ecs.add_component(new_player, component::Rotation(90));
    _ecs.add_component(new_player, component::Health(100));
    _ecs.add_component(new_player, component::Damage(20));
    _ecs.add_component(new_player, component::Score());

    std::cout << "New player connected !" << std::endl;
    send_animated_drawable_snapshots_for_specific_player_by_room(new_player, _ecs.get_components<component::AnimatedDrawable>());
    send_animated_drawable_snapshot_to_all_players(new_player, _ecs.get_components<component::AnimatedDrawable>(), _ecs.get_components<component::Endpoint>());
    send_all_entity_drawables_to_specific_player_by_room(new_player);
    send_highscore_to_specific_client(new_player);
    send_all_scale_to_player_by_room(new_player);
    send_scale_to_all_players(new_player, _ecs.get_components<component::Scale>(), _ecs.get_components<component::Endpoint>());
    return new_player;
}

void Server::send_all_scale_to_player(entity_t entity)
{
    auto scale = _ecs.get_components<component::Scale>();

    for (int i = 0; i < scale.size(); ++i) {
        if (!scale[i].has_value())
            continue;
        ScaleSnapshot to_send(15, i, *scale[i], _packet_id);
        _scale_packets.push_back(to_send);
        _packet_id++;
        send_data_to_client_by_entity(to_send, entity);
    }
}

void Server::send_all_scale_to_player_by_room(entity_t entity)
{
    auto scale = _ecs.get_components<component::Scale>();
    auto rooms = _ecs.get_components<component::Room>();

    for (int i = 0; i < scale.size(); ++i) {
        if (!scale[i].has_value() || !rooms[entity].has_value() || !rooms[i].has_value() || (rooms[entity]->_name != rooms[i]->_name))
            continue;
        ScaleSnapshot to_send(15, i, *scale[i], _packet_id);
        _packet_id++;
        _scale_packets.push_back(to_send);
        send_data_to_client_by_entity(to_send, entity);
    }
}

void Server::send_scale_to_all_players(entity_t entity, sparse_array<component::Scale> &scl, sparse_array<component::Endpoint> &edp)
{
    auto scale = scl[entity];

    if (!scale.has_value())
        return;
    ScaleSnapshot to_send(15, entity, *scale, 0);
    auto &rooms = _ecs.get_components<component::Room>();
    send_data_to_all_clients_by_room(to_send, _scale_packets, edp, rooms, rooms[entity]->_name);
}

void Server::send_all_entity_drawables_to_specific_player_by_room(entity_t player)
{
    auto drawables = _ecs.get_components<component::Drawable>();
    auto rooms = _ecs.get_components<component::Room>();

    for (int i = 0; i < drawables.size(); ++i) {
        if (!drawables[i].has_value() || !rooms[player].has_value() || !rooms[i].has_value() || (rooms[player]->_name != rooms[i]->_name))
            continue;
        DrawableSnapshot to_send(6, player, drawables[i].value()._path, _packet_id);
        _packet_id += 1;
        _drawable_packets.push_back(to_send);
        send_data_to_client_by_entity(to_send, player);
    }
}

void Server::send_all_entity_drawables_to_specific_player(entity_t player)
{
    auto drawables = _ecs.get_components<component::Drawable>();
    auto rooms = _ecs.get_components<component::Room>();

    for (int i = 0; i < drawables.size(); ++i) {
        if (!drawables[i].has_value() || !rooms[player].has_value() || !rooms[i].has_value() || (rooms[player]->_name != rooms[i]->_name))
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
    _socket.non_blocking(true);
    try {
        size_t bytesRead = _socket.receive_from(asio::buffer(receivedData), _remote_endpoint);
        receivedData.resize(bytesRead);
    } catch (std::exception &e) {
        receivedData.resize(0);
    }

    return receivedData;
}

void Server::send_health_to_specific_client(sparse_array<component::Position> &pos, sparse_array<component::Endpoint> &edp)
{
    auto &health = _ecs.get_components<component::Health>();
    auto player = _ecs.get_components<component::Endpoint>();
    for (size_t i = 0; i < pos.size(); i++) {
        if (player[i].has_value() && health[i].has_value()) {
            HealthMessage to_send(23, health[i].value()._health, _packet_id);
            printf("health ======== %d\n", health[i].value()._health);
            _packet_id++;
            _health_packets_to_send.push_back(to_send);
            send_data_to_client_by_entity(to_send, i);
        }
    }
    auto score = _ecs.get_components<component::Score>();
}

void Server::send_score_to_specific_client(sparse_array<component::Position> &pos, sparse_array<component::Endpoint> &edp)
{
    auto player = _ecs.get_components<component::Endpoint>();
    for (size_t i = 0; i < pos.size(); i++) {
        if (player[i].has_value() && score[i].has_value()) {
            ScoreMessage to_send(24, score[i]->_score, _packet_id);
            printf("score ======== %d\n", score[i]->_score);
            _packet_id++;
            _score_packets_to_send.push_back(to_send);
            send_data_to_client_by_entity(to_send, i);
        }
    }
}

void Server::send_position_snapshots_for_all_players(sparse_array<component::Position> &pos, sparse_array<component::Endpoint> &edp)
{
    std::vector<SnapshotPosition> to_send = {};
    for (size_t i = 0; i < pos.size(); i++) {
        if (pos[i].has_value()) {
            to_send.push_back(SnapshotPosition(4, i, component::Position(pos[i].value().x, pos[i].value().y), 0));

        }
    }
    for (SnapshotPosition &pos: to_send) {
        auto &rooms = _ecs.get_components<component::Room>();
        send_data_to_all_clients_by_room(pos, _position_packets, edp, rooms, rooms[pos.entity]->_name);
    }
}

void Server::send_animated_drawable_update_to_all_clients(entity_t entity, std::string state, sparse_array<component::Endpoint> &edp)
{
    if (state.size() > 15) {
        std::cout << "ERROR STATE SIZE TOO BIG TO BE STORED\n";
        return;
    }
    AnimatedStateUpdateMessage to_send(14, entity, state, 0);
    auto &rooms = _ecs.get_components<component::Room>();
    send_data_to_all_clients_by_room(to_send, _animated_drawable_update_packets, edp, rooms, rooms[entity]->_name);
}

void Server::send_animated_drawable_snapshot_to_all_players(entity_t entity, sparse_array<component::AnimatedDrawable> &dra, sparse_array<component::Endpoint> &edp)
{
    auto &animatedDrawable = dra[entity];
    if (animatedDrawable.has_value()) {
        AnimatedDrawableSnapshot snap_ad(
            13,
            entity,
            animatedDrawable->_path,
            animatedDrawable->_nbSprites,
            animatedDrawable->_spriteSize,
            animatedDrawable->_gaps,
            animatedDrawable->_firstOffset,
            animatedDrawable->_currentIdx,
            animatedDrawable->_anims,
            animatedDrawable->_state,
            _packet_id
        );
        auto &rooms = _ecs.get_components<component::Room>();
        send_data_to_all_clients_by_room(snap_ad, _animated_drawable_packets, edp, rooms, rooms[entity]->_name);
    }
}

void Server::send_animated_drawable_snapshots_for_specific_player_by_room(entity_t entity, sparse_array<component::AnimatedDrawable> dra)
{
    auto rooms = _ecs.get_components<component::Room>();
    for (size_t i = 0; i < dra.size(); i++) {
        if (!dra[i].has_value() || !rooms[entity].has_value() || !rooms[i].has_value() || (rooms[entity]->_name != rooms[i]->_name))
            continue;
        AnimatedDrawableSnapshot snap_ad(
            13,
            i,
            dra[i].value()._path,
            dra[i].value()._nbSprites,
            dra[i].value()._spriteSize,
            dra[i].value()._gaps,
            dra[i].value()._firstOffset,
            dra[i].value()._currentIdx,
            dra[i].value()._anims,
            dra[i].value()._state,
            _packet_id
        );
        _packet_id++;
        _animated_drawable_packets.push_back(snap_ad);
        send_data_to_client_by_entity(snap_ad, entity);
    }
}

void Server::send_animated_drawable_snapshots_for_specific_player(entity_t entity, sparse_array<component::AnimatedDrawable> dra)
{
    for (size_t i = 0; i < dra.size(); i++) {
        if (dra[i].has_value()) {
            AnimatedDrawableSnapshot snap_ad(
                13,
                i,
                dra[i].value()._path,
                dra[i].value()._nbSprites,
                dra[i].value()._spriteSize,
                dra[i].value()._gaps,
                dra[i].value()._firstOffset,
                dra[i].value()._currentIdx,
                dra[i].value()._anims,
                dra[i].value()._state,
                _packet_id
            );
            _packet_id++;
            _animated_drawable_packets.push_back(snap_ad);
            send_data_to_client_by_entity(snap_ad, entity);
        }
    }
}

void Server::send_entity_drawable_to_all_players(entity_t entity, sparse_array<component::Drawable> &dra, sparse_array<component::Endpoint> &edp)
{
    component::Drawable drawable = dra[entity].value();
    DrawableSnapshot to_send(6, entity, drawable._path, 0);
    auto &rooms = _ecs.get_components<component::Room>();
    send_data_to_all_clients_by_room(to_send, _drawable_packets, edp, rooms, rooms[entity]->_name);
}

void Server::send_death_event_to_all_players(entity_t entity, sparse_array<component::Endpoint> &edp)
{
    DeathEventMessage evt(16, entity, 0);
    auto &rooms = _ecs.get_components<component::Room>();
    send_data_to_all_clients_by_room(evt, _death_packets, edp, rooms, rooms[entity]->_name);
}

void Server::recieve_from_client()
{
    std::vector<char> client_msg = recieve_raw_data_from_client();
    if (client_msg.size() < sizeof(BaseMessage)) {
        return;
    }
    entity_t player_entity = get_player_entity_from_connection_address(_remote_endpoint);
    BaseMessage *baseMsg = reinterpret_cast<BaseMessage *>(client_msg.data());

    if (player_entity == -1 || baseMsg->id == 5) {
        if (baseMsg->id == 5 || baseMsg->id == 17 || baseMsg->id == 21 || baseMsg->id == 22 || baseMsg->id == 2) {
            (this->*_messageParser[baseMsg->id])(client_msg, 1);
            return;
        } else
            return;
    }
    if (_messageParser.find(baseMsg->id) == _messageParser.end())
        throw ArgumentError("ERROR: Invalid event recieved: " + std::to_string(baseMsg->id) + ".");
    (this->*_messageParser[baseMsg->id])(client_msg, player_entity);
    return;
}

int Server::receive_room_join_event(std::vector<char>& client_msg, entity_t _)
{
    RoomJoinMessage *joinMsg = reinterpret_cast<RoomJoinMessage *>(client_msg.data());

    if (_lobbies.find(std::string(joinMsg->room_name)) == _lobbies.end()) {
        RoomJoinMessage to_send(22, "", _packet_id);
        _packet_id++;
        // _room_join_packets.push_back(to_send);
        _socket.send_to(asio::buffer(&to_send, sizeof(RoomJoinMessage)), _remote_endpoint);
        return -1;
    }
    RoomJoinMessage to_send(22, std::string(joinMsg->room_name), _packet_id);
    _packet_id++;
    _room_join_packets.push_back(to_send);
    _resend_packets_endpoints[to_send.packet_id] = _remote_endpoint;
    _socket.send_to(asio::buffer(&to_send, sizeof(RoomJoinMessage)), _remote_endpoint);
    return 0;
}

int Server::receive_room_creation_event(std::vector<char>& client_msg, entity_t _) {
    RoomCreationMessage *creationMsg = reinterpret_cast<RoomCreationMessage *>(client_msg.data());

    if (_lobbies.find(std::string(creationMsg->room_name)) != _lobbies.end()) {
        RoomCreationMessage to_send(21, "", "", _packet_id);
        _packet_id++;
        _room_creation_packets.push_back(to_send);
        _resend_packets_endpoints[to_send.packet_id] = _remote_endpoint;
        _socket.send_to(asio::buffer(&to_send, sizeof(RoomCreationMessage)), _remote_endpoint);
        return -1;
    }
    _lobbies[std::string(creationMsg->room_name)] = std::string(creationMsg->username);
    RoomCreationMessage to_send(21, std::string(creationMsg->username), std::string(creationMsg->room_name), _packet_id);
    _packet_id++;
    _room_creation_packets.push_back(to_send);
    _resend_packets_endpoints[to_send.packet_id] = _remote_endpoint;
    _socket.send_to(asio::buffer(&to_send, sizeof(RoomCreationMessage)), _remote_endpoint);
    return 0;
}

int Server::recieve_packet_confirm(std::vector<char> & client_msg, entity_t _) {
    ConfirmationMessage *confirmMsg = reinterpret_cast<ConfirmationMessage *>(client_msg.data());
    int id = confirmMsg->packet_id;

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
    _room_creation_packets.erase(
        std::remove_if(_room_creation_packets.begin(), _room_creation_packets.end(), [id](const RoomCreationMessage& snapshot) {
            return snapshot.packet_id == id;
        }
        ),
        _room_creation_packets.end()
    );
    _room_join_packets.erase(
        std::remove_if(_room_join_packets.begin(), _room_join_packets.end(), [id](const RoomJoinMessage& snapshot) {
            return snapshot.packet_id == id;
        }
        ),
        _room_join_packets.end()
    );
    return 0;
}

int Server::recieve_client_event(std::vector<char> &client_msg, entity_t player_entity)
{
    if (client_msg.size() < sizeof(EventMessage))
        return -1;
    EventMessage *event = reinterpret_cast<EventMessage *>(client_msg.data());
    std::pair<int, int> to_move = get_position_change_for_event(player_entity, event->event);
    if (to_move.first != 0 || to_move.second != 0) {
        _listener.addEvent(new UpdatePositionEvent(player_entity, to_move));
        _listener.addEvent(new PositionStayInWindowBounds(player_entity, {0, 1920, 0, 1080}));
    }
    return 0;
}

int Server::recieve_connection_event(std::vector<char> &client_msg, entity_t _)
{
    if (client_msg.size() < sizeof(JoinGameMessage))
        return -1;
    JoinGameMessage *msg = reinterpret_cast<JoinGameMessage *>(client_msg.data());
    std::cerr << connect_player(_remote_endpoint, std::string(msg->username), std::string(msg->room_name)) << std::endl;
    return 0;
}

int Server::recieve_disconnection_event(std::vector<char> &client_msg, entity_t player_entity)
{
    _listener.addEvent(new DeathEvent(player_entity, 0));
    return 0;
}

int Server::receive_login_event(std::vector<char> &client_msg, entity_t player_entity) {
    if (client_msg.size() < sizeof(LoginMessage))
        return -1;
    LoginMessage *snapshot = reinterpret_cast<LoginMessage *>(client_msg.data());
    bool response = true;
    if (snapshot->logintype == 0)
        response = signUp(snapshot->username, snapshot->password);
    else if (snapshot->logintype == 1)
        response = signIn(snapshot->username, snapshot->password);
    LoginResponse resp(8, response, snapshot->logintype, _packet_id);
    _socket.send_to(asio::buffer(&resp, sizeof(LoginResponse)), _remote_endpoint);
    _packet_id += 1;
    return 0;
}

int Server::receive_friend_event(std::vector<char> &client_msg, entity_t player_entity)
{
    if (client_msg.size() < sizeof(FriendsMessage)) {
        return -1;
    }
    FriendsMessage *snapshot = reinterpret_cast<FriendsMessage *>(client_msg.data());
    std::vector<std::string> friends = displayFriends(snapshot->username, player_entity);
    _packet_id++;
    return 0;
}

int Server::receive_add_friend_event(std::vector<char>& client_msg, entity_t player_entity)
{
    if (client_msg.size() < sizeof(AddFriendsMessage))
        return -1;
    AddFriendsMessage *snapshot = reinterpret_cast<AddFriendsMessage *>(client_msg.data());
    std::cout << snapshot->friendName << std::endl;
    std::cout << snapshot->username << std::endl;
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
    ChatMessage reponse(12, snapshot->name, snapshot->content, _packet_id);
    
    auto &rooms = _ecs.get_components<component::Room>();
    send_data_to_all_clients_by_room(reponse, _chat_packets, _ecs.get_components<component::Endpoint>(), rooms, rooms[player_entity]->_name);
    return 0;
}

Server::~Server() {
    // if (_send_thread.joinable())
    //     _send_thread.join();
}

template <typename T>
void Server::send_data_to_all_clients(T& structure, std::vector<T>& packets_to_send, sparse_array<component::Endpoint> &edp) {
    for (size_t i = 0; i < edp.size(); i++) {
        if (edp[i].has_value()) {
            structure.packet_id = _packet_id;
            _packet_id += 1;
            packets_to_send.push_back(structure);
            if (edp[i].has_value())
                _socket.send_to(asio::buffer(&structure, sizeof(structure)), edp[i]->_endpoint);
        }
    }
}

template <typename T>
void Server::send_data_to_all_clients_by_room(T& structure, std::vector<T>& packets_to_send, sparse_array<component::Endpoint> &edp, sparse_array<component::Room> &rms, std::string room) {
    for (size_t i = 0; i < edp.size(); i++) {
        if (edp[i].has_value() && ((rms[i].has_value() && (rms[i]->_name == room)))) { //  || !rms[i].has_value()
            structure.packet_id = _packet_id;
            _packet_id += 1;
            if (edp[i].has_value()) {
                packets_to_send.push_back(structure);
                _resend_packets_endpoints[structure.packet_id] = edp[i]->_endpoint;
                _socket.send_to(asio::buffer(&structure, sizeof(structure)), edp[i]->_endpoint);
            }
        }
    }
}

template <typename T>
void Server::send_data_to_all_clients_except_me(T& structure, sparse_array<component::Endpoint> &edp) {
    for (size_t i = 0; i < edp.size(); i++) {
        if (edp[i].has_value() && edp[i].value()._endpoint != _remote_endpoint) {
            if (edp[i].has_value())
                _socket.send_to(asio::buffer(&structure, sizeof(structure)), edp[i]->_endpoint);
        }
    }
}

template <typename T>
void Server::resend_packets(std::vector<T> &packets, sparse_array<component::Endpoint> &edp) {
    for (auto& packet : packets) {
        _socket.send_to(asio::buffer(&packet, sizeof(packet)), _resend_packets_endpoints[packet.packet_id]);
    }
}
