/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** main
*/

#include "Server.hpp"
#include <iostream>
#include <thread>

Server::Server(boost::asio::io_service &service, int port, registry &ecs): _service(service), _socket(service, udp::endpoint(udp::v4(), port)), _ecs(ecs)
{
    _tpool.emplace_back([this, &service]() { service.run(); });
    recieve_from_client();
}

void Server::recieve_from_client() {
    data_struct structure;
    receive_datas(structure);
    if (structure.id == 1)
        std::cout << "Event" << std::endl;
    if (structure.id == 2) {
        snapshot snap = {4, std::string("Infos sur les entités")};
        send_datas(snap);
        std::cout << "Client connected" << std::endl;
    }
    if (structure.id == 3)
        std::cout << "Disconnect" << std::endl;
    recieve_from_client();
}

Server::~Server() {}

template <typename T>
void Server::send_datas(const T& structure) {
    _socket.send_to(boost::asio::buffer(&structure, sizeof(structure)), _remote_endpoint);
}

template <typename T>
void Server::receive_datas(T& structure) {
    _socket.receive_from(boost::asio::buffer(&structure, sizeof(structure)), _remote_endpoint);
}
