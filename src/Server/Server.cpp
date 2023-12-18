/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** main
*/

#include "Server.hpp"
#include <iostream>
#include <thread>

struct data_struct {
    int id;
    sf::Event::EventType eventType;
};

Server::Server(boost::asio::io_service &service, int port): _service(service), _socket(service, udp::endpoint(udp::v4(), port))
{
    _tpool.emplace_back([this, &service]() { service.run(); });
    data_struct structure;
    receive_datas(structure);
    if (structure.id == 1)
        std::cout << "Event" << std::endl;
    if (structure.id == 2)
        std::cout << "Client connected" << std::endl;
    if (structure.id == 3)
        std::cout << "Disconnect" << std::endl;
}

Server::~Server() {}

template <typename T>
void Server::receive_datas(T& structure) {
    _socket.receive_from(boost::asio::buffer(&structure, sizeof(structure)), _remote_endpoint);
}
