/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** main
*/

#include "Server.hpp"
#include "KeyEventMapping.hpp"

bool can_read = true;

std::pair<int, int> Server::get_position_change_for_event(entity_t entity, int event)
{
    std::cout << "CODE: " << event << " !!!!\n";
    if (event == KeyIds["Up"])
        return {0, -30};
    if (event == KeyIds["Down"])
        return {0, 30};
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
    _ecs.add_component(new_player, component::Position(10.0f, 10.0f));
    _ecs.add_component(new_player, component::Velocity(0.0f, 0.0f));
    _ecs.add_component(new_player, component::ResetOnMove());
    _ecs.add_component(new_player, component::Controllable());
    _ecs.add_component(new_player, component::Heading());
    _ecs.add_component(new_player, component::Drawable("temp/assets/textures/sprites/r-typesheet42.gif"));
    _ecs.add_component(new_player, component::Endpoint(player_endpoint));
    _ecs.add_component(new_player, component::Scale(0.1));
    _ecs.add_component(new_player, component::Rotation(90));
    _ecs.add_component(new_player, component::Health(100));
    _ecs.add_component(new_player, component::Damage(20));
    _ecs.add_component(new_player, component::Score());
    std::cout << "New player connected !" << std::endl;
    auto all_players = _ecs.get_components<component::Endpoint>();
    send_entity_drawable_to_all_players(new_player);
    send_all_entity_drawables_to_specific_player(new_player);
    send_highscore_to_specific_client(new_player);
    return new_player;
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
    sparse_array<component::Position> pos = _ecs.get_components<component::Position>();
    for (size_t i = 0; i < pos.size(); i++) {
        if (pos[i].has_value()) {
            // std::cout << "position: x "  << pos[i].value().x << ", y " << pos[i].value().y << std::endl;
            SnapshotPosition snap_p(4, i, component::Position(pos[i].value().x, pos[i].value().y), _packet_id);
            _position_packets.push_back(snap_p);
            _packet_id += 1;
            send_data_to_all_clients<SnapshotPosition>(snap_p);
        }
    }
}

void Server::send_entity_drawable_to_all_players(entity_t entity)
{
    sparse_array<component::Drawable> drawables = _ecs.get_components<component::Drawable>();
    component::Drawable drawable = drawables[entity].value();
    DrawableSnapshot to_send(6, entity, drawable._path, _packet_id);
    _packet_id += 1;
    _drawable_packets.push_back(to_send);
    send_data_to_all_clients<DrawableSnapshot>(to_send);
}

void Server::recieve_from_client()
{
    std::vector<char> client_msg = recieve_raw_data_from_client();
    if (client_msg.size() < sizeof(BaseMessage))
        return;
    entity_t player_entity = get_player_entity_from_connection_address(_remote_endpoint);
    BaseMessage *baseMsg = reinterpret_cast<BaseMessage *>(client_msg.data());

    if (player_entity == -1) {
        player_entity = connect_player(_remote_endpoint);
    }
    // std::cout << "message id: " << baseMsg->id << std::endl;
    if (_messageParser.find(baseMsg->id) == _messageParser.end())
        throw ArgumentError("ERROR: Invalid event recieved: " + std::to_string(baseMsg->id) + ".");
    (this->*_messageParser[baseMsg->id])(client_msg, player_entity);
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
    return 0;
}

int Server::recieve_client_event(std::vector<char> &client_msg, entity_t player_entity)
{
    if (client_msg.size() < sizeof(EventMessage))
        return -1;
    EventMessage *event = reinterpret_cast<EventMessage *>(client_msg.data());
    std::cout << "New event recieved from: " << _remote_endpoint << std::endl;
    std::cout << "event recieved: " << event->event << std::endl;
    _listener.addEvent(new UpdatePositionEvent(player_entity, get_position_change_for_event(player_entity, event->event)));
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
    send_data_to_all_clients<ChatMessage>(reponse);
    return 0;
}


Server::~Server() {
    if (_send_thread.joinable())
        _send_thread.join();
}

template <typename T>
void Server::send_data_to_all_clients(T& structure) {
    sparse_array<component::Endpoint> all_endpoints = _ecs.get_components<component::Endpoint>();
    for (size_t i = 0; i < all_endpoints.size(); i++) {
        if (all_endpoints[i].has_value()) {
            while (!can_mod) continue;
            _socket.send_to(asio::buffer(&structure, sizeof(structure)), all_endpoints[i].value()._endpoint);
        }
    }
}

template <typename T>
void Server::send_data_to_all_clients_except_me(T& structure) {
    sparse_array<component::Endpoint> all_endpoints = _ecs.get_components<component::Endpoint>();
    for (size_t i = 0; i < all_endpoints.size(); i++) {
        if (all_endpoints[i].has_value() && all_endpoints[i].value()._endpoint != _remote_endpoint) {
            while (!can_mod) continue;
            _socket.send_to(asio::buffer(&structure, sizeof(structure)), all_endpoints[i].value()._endpoint);
        }
    }
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

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        if (counter >= 20) {
            can_mod = false;
            resend_packets<SnapshotPosition>(_position_packets);
            resend_packets<DrawableSnapshot>(_drawable_packets);
            resend_packets<HighScoreMessage>(_highscore_packets);
            can_mod = true;
            counter = 0;
        }
        send_position_snapshots_for_all_players();
        ++counter;
    }
}
