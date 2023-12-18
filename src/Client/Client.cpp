/*
** EPITECH PROJECT, 2023
** R-TYPE
** File description:
** Client.cpp
*/

#include "Client.hpp"

struct MaStructure {
    int entier;
    double reel;
    char chaine[20];
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

// void Client::send_datas()
// {
//     std::string message;
//     std::getline(std::cin, message);
//     _socket.send_to(boost::asio::buffer(message), _server_endpoint);
// }

void Client::receive_datas()
{
    char receive_buffer[1024];
    udp::endpoint sender_endpoint;
    size_t bytes_received = _socket.receive_from(boost::asio::buffer(receive_buffer), sender_endpoint);
    receive_buffer[bytes_received] = '\0';
    std::cout << "Received from " << sender_endpoint.address().to_string() << ": " << receive_buffer << std::endl;
}

int Client::run()
{
    while (true) {
        MaStructure structure = {42, 3.14, "Bonjour"};
        send_datas(structure);
        receive_datas();
    }
    return 0;
}