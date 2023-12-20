/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** main
*/

#include "Server.hpp"

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

Server::Server(boost::asio::io_service &service, int port, registry &ecs, rtype::event::EventListener &listener): _service(service), _socket(service, udp::endpoint(udp::v4(), port)), _ecs(ecs), _listener(listener), _send_thread(&Server::sendPositionPackagesPeriodically, this)
{
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
    _ecs.add_component(new_player, component::Velocity(0.0f, 0.0f, true));
    _ecs.add_component(new_player, component::Player(100, 20));
    _ecs.add_component(new_player, component::Heading());
    _ecs.add_component(new_player, component::Endpoint(player_endpoint));
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

void Server::recieve_from_client()
{
    data_struct structure;
    receive_datas(structure);
    entity_t player_entity = get_player_entity_from_connection_address(_remote_endpoint);

    if (player_entity == -1) {
        player_entity = connect_player(_remote_endpoint);
    }
    if (structure.id == 1) {
        std::cout << "New event recieved from: " << _remote_endpoint << std::endl;
        _listener.addEvent(new rtype::event::UpdatePositionEvent(player_entity, get_position_change_for_event(player_entity, structure.event)));
        sparse_array<component::Position> pos = _ecs.get_components<component::Position>();
        for (size_t i = 0; i < pos.size(); i++) {
            if (pos[i].has_value()) {
                snapshot_position snap_p = {4, i, component::Position(pos[i].value().x, pos[i].value().y)};
                send_data_to_all_clients<snapshot_position>(snap_p);
            }
        }
        // snapshot_velocity snap_v = {5, _ecs.get_components<component::Velocity>()};
        // send_data_to_all_clients<snapshot_velocity>(snap_v);
        // snapshot_player snap_pl = {6, _ecs.get_components<component::Player>()};
        // send_data_to_all_clients<snapshot_player>(snap_pl);
    }
    if (structure.id == 3)
        _ecs.kill_entity(player_entity);
    if (structure.id == 5) {
            int id = structure.package_id;
            _position_packages.erase(
                std::remove_if(_position_packages.begin(), _position_packages.end(), [id](const snapshot_position& snapshot) {
                        return snapshot.package_id == id;
                }
                ),
                _position_packages.end()
            );
        std::cout << structure.package_id << std::endl;
    }

    recieve_from_client();
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
            structure.package_id = _package_id;
            _package_id += 1;
            _position_packages.push_back(structure);
            _socket.send_to(boost::asio::buffer(&structure, sizeof(structure)), all_endpoints[i].value()._endpoint);
        }
    }
}

void Server::sendPositionPackagesPeriodically() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        for (auto& snapshot : _position_packages) {
            sparse_array<component::Endpoint> all_endpoints = _ecs.get_components<component::Endpoint>();
            for (size_t i = 0; i < all_endpoints.size(); i++) {
                if (all_endpoints[i].has_value())
                    _socket.send_to(boost::asio::buffer(&snapshot, sizeof(snapshot)), all_endpoints[i].value()._endpoint);
            }
        }
    }
}

template <typename T>
void Server::receive_datas(T& structure) {
    _socket.receive_from(boost::asio::buffer(&structure, sizeof(structure)), _remote_endpoint);
}
