/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** main
*/

#include "Server.hpp"
#include <iostream>
#include <thread>

std::pair<int, int> get_position_change_for_event(sf::Event::EventType evtType)
{
    return {5, 5};
}

Server::Server(boost::asio::io_service &service, int port, registry &ecs, rtype::event::EventListener &listener): _service(service), _socket(service, udp::endpoint(udp::v4(), port)), _ecs(ecs), _listener(listener)
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
        _listener.addEvent(new rtype::event::UpdatePositionEvent(player_entity, get_position_change_for_event(structure.eventType)));
        snapshot_position snap_p = {4, convert_to_map(_ecs.get_components<component::Position>())};
        send_data_to_all_clients<snapshot_position>(snap_p);
        // snapshot_velocity snap_v = {5, _ecs.get_components<component::Velocity>()};
        // send_data_to_all_clients<snapshot_velocity>(snap_v);
        // snapshot_player snap_pl = {6, _ecs.get_components<component::Player>()};
        // send_data_to_all_clients<snapshot_player>(snap_pl);
    }
    if (structure.id == 3)
        _ecs.kill_entity(player_entity);
    if (structure.id == 5);

    recieve_from_client();
}

Server::~Server() {}

template <typename T>
void Server::send_data_to_all_clients(const T& structure) {
    sparse_array<component::Endpoint> all_endpoints = _ecs.get_components<component::Endpoint>();
    for (size_t i = 0; i < all_endpoints.size(); i++) {
        if (all_endpoints[i].has_value()) {
            _socket.send_to(boost::asio::buffer(&structure, sizeof(structure)), all_endpoints[i].value()._endpoint);
        }
    }
}

template <typename T>
void Server::receive_datas(T& structure) {
    _socket.receive_from(boost::asio::buffer(&structure, sizeof(structure)), _remote_endpoint);
}
