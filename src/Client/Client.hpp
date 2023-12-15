/*
** EPITECH PROJECT, 2023
** R-TYPE
** File description:
** Client.hpp
*/

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

class Client {
    public:
        Client(std::string ip, int port);
        ~Client();
        int run();
        void send_datas();
        void receive_datas();

    private:
        boost::asio::io_context _io_context;
        udp::endpoint _server_endpoint;
        udp::socket _socket;
};

#endif // CLIENT_HPP