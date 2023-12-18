/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** main
*/

#ifndef SERVER_HPP
#define SERVER_HPP
#include <iostream>
#include <array>
#include <boost/asio.hpp>
#include "ecs/Registry.hpp"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

using boost::asio::ip::udp;


class Server {
    public:
        Server(boost::asio::io_service &service, int port, registry &ecs);
        ~Server();
        void recieve_from_client();
        template <typename T>
        void receive_datas(T& structure);

    private:
        std::array<char, 1024> _buf;
        boost::asio::io_service::work _service;
        udp::endpoint _remote_endpoint;
        std::vector<std::thread> _tpool;
        udp::socket _socket;
        registry &_ecs;
};

#endif // SERVER_HPP
