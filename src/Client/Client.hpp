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
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

using boost::asio::ip::udp;

struct data_struct {
    int id;
    sf::Event::EventType eventType;
};

class Client {
    public:
        Client(std::string ip, int port);
        ~Client();
        int run();
        template <typename T>
        void send_datas(const T& structure);

        template <typename T>
        void receive_datas(T& structure);

    private:
        boost::asio::io_context _io_context;
        udp::endpoint _server_endpoint;
        udp::socket _socket;
        data_struct _send_structure;
};

#endif // CLIENT_HPP