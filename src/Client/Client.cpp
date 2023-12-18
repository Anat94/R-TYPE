/*
** EPITECH PROJECT, 2023
** R-TYPE
** File description:
** Client.cpp
*/

#include "Client.hpp"

Client::Client(std::string ip, int port)
    : _io_context(),
      _socket(_io_context, udp::endpoint(udp::v4(), 0)),
      _server_endpoint(udp::endpoint(boost::asio::ip::address::from_string(ip), port))
{
    _send_structure.id = 2;
    send_datas(_send_structure);
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
    sf::RenderWindow window(sf::VideoMode(800, 600), "R-TYPE");
    std::thread serverThread([&]() {
        receive_datas(_receive_structure);
    });
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                _send_structure.id = 3;
                send_datas(_send_structure);
                window.close();
                std::cout << _receive_structure.data << std::endl;
            }
            if (std::find(eventsToPrint.begin(), eventsToPrint.end(), event.type) != eventsToPrint.end()) {
                _send_structure.id = 1;
                _send_structure.eventType = event.type;
                send_datas(_send_structure);
            }
        }
    }
    return 0;
}