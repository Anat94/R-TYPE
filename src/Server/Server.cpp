/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** main
*/

#include "Server.hpp"

bool can_mod = true;

std::pair<int, int> Server::get_position_change_for_event(entity_t entity, sf::Event event)
{
    if (event.key.code == sf::Keyboard::Up)
        return {0, -30};
    if (event.key.code == sf::Keyboard::Down)
        return {0, 30};
    if (event.key.code == sf::Keyboard::Left)
        return {-30, 0};
    if (event.key.code == sf::Keyboard::Right)
        return {30, 0};
    if (event.key.code == sf::Keyboard::Space)
        _listener.addEvent(new rtype::event::ShootEvent(entity, -1));
    return {0, 0};
}

Server::Server(asio::io_context& service, int port, registry& ecs, rtype::event::EventListener& listener)
    : _service(service),
      _socket(service, udp::endpoint(udp::v4(), port)),
      _ecs(ecs),
      _listener(listener),
      _send_thread(&Server::sendPositionPackagesPeriodically, this)
{
    _tpool.emplace_back([this, &service]() {
        service.run();
    });
    try {
        // connectToDB();
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
    _ecs.add_component(new_player, component::Endpoint(player_endpoint));
    _ecs.add_component(new_player, component::Scale(0.1));
    _ecs.add_component(new_player, component::Rotation(90));
    _ecs.add_component(new_player, component::Health(100));
    _ecs.add_component(new_player, component::Damage(20));
    _ecs.add_component(new_player, component::Score());
    std::cout << "New player connected !" << std::endl;
    return new_player;
}

std::map<entity_t, std::pair<float, float>> convert_to_map(sparse_array<component::Position> pos)
{
    std::map<entity_t, std::pair<float, float>> res = {};
    for (int i = 0; i < pos.size(); i++) {
        if (pos[i].has_value()) {
            res[i] = std::make_pair(pos[i].value().x, pos[i].value().y);
        }
    }
    return res;
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
            SnapshotPosition snap_p(4, i, component::Position(pos[i].value().x, pos[i].value().y), 0);
            send_data_to_all_clients<SnapshotPosition>(snap_p);
        }
    }
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
    std::cout << "message id: " << baseMsg->id << std::endl;
    if (_messageParser.find(baseMsg->id) == _messageParser.end())
        throw ArgumentError("ERROR: Invalid event recieved: " + std::to_string(baseMsg->id) + ".");
    (this->*_messageParser[baseMsg->id])(client_msg, player_entity);
    recieve_from_client();
}

void Server::recieve_packet_confirm(std::vector<char> & client_msg, entity_t _) {
    ConfirmationMessage *confirmMsg = reinterpret_cast<ConfirmationMessage *>(client_msg.data());
    int id = confirmMsg->packet_id;

    while (!can_mod) continue;
    _position_packages.erase(
        std::remove_if(_position_packages.begin(), _position_packages.end(), [id](const SnapshotPosition& snapshot) {
            return snapshot.packet_id == id;
        }
        ),
        _position_packages.end()
    );
}

void Server::recieve_client_event(std::vector<char> &client_msg, entity_t player_entity)
{
    if (client_msg.size() < sizeof(EventMessage))
        return;
    EventMessage *event = reinterpret_cast<EventMessage *>(client_msg.data());
    std::cout << "New event recieved from: " << _remote_endpoint << std::endl;
    _listener.addEvent(new rtype::event::UpdatePositionEvent(player_entity, get_position_change_for_event(player_entity, event->event)));
    send_position_snapshots_for_all_players();
}

void Server::recieve_connection_event(std::vector<char> &client_msg, entity_t player_entity)
{
    static_cast<void>(client_msg);
    static_cast<void>(player_entity);
}

void Server::recieve_disconnection_event(std::vector<char> &client_msg, entity_t player_entity)
{
    _ecs.kill_entity(player_entity);
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
            structure.packet_id = _packet_id;
            _packet_id += 1;
            while (!can_mod) continue;
            _position_packages.push_back(structure);
            _socket.send_to(asio::buffer(&structure, sizeof(structure)), all_endpoints[i].value()._endpoint);
        }
    }
}

void Server::sendPositionPackagesPeriodically() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        can_mod = false;
        for (auto& snapshot : _position_packages) {
            sparse_array<component::Endpoint> all_endpoints = _ecs.get_components<component::Endpoint>();
            for (size_t i = 0; i < all_endpoints.size(); i++) {
                if (all_endpoints[i].has_value())
                    _socket.send_to(asio::buffer(&snapshot, sizeof(snapshot)), all_endpoints[i].value()._endpoint);
            }
        }
        can_mod = true;
    }
}
