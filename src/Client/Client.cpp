/*
** EPITECH PROJECT, 2023
** R-TYPE
** File description:
** Client.cpp
*/

#include "Client.hpp"

struct data_struct {
    int id;
    sf::Event::EventType eventType;
};

Client::Client(std::string ip, int port)
    : _io_context(),
      _socket(_io_context, udp::endpoint(udp::v4(), 0)),
      _server_endpoint(udp::endpoint(boost::asio::ip::address::from_string(ip), port))
{
}

Client::~Client()
{
}

template <typename T>
void Client::send_datas(const T& structure) {
    _socket.send_to(boost::asio::buffer(&structure, sizeof(structure)), _server_endpoint);
}

template <typename T>
void Client::receive_datas(T& structure) {
    _socket.receive_from(boost::asio::buffer(&structure, sizeof(structure)), _server_endpoint);
}

int Client::run()
{
    data_struct send_structure = {};
    data_struct receive_structure;

    sf::RenderWindow window(sf::VideoMode(800, 600), "R-TYPE");
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                send_structure.id = 3;
                send_datas(send_structure);
                window.close();
            }
        }
    }
    return 0;
}