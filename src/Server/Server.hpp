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
#include "ecs/Events.hpp"
#include "ecs/ZipperIterator.hpp"
#include <boost/asio.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

using boost::asio::ip::udp;

struct snapshot {
    int id;
    std::string data;
};

struct data_struct {
    int id;
    sf::Event::EventType eventType;
};

class Server {
    public:
        Server(boost::asio::io_service &service, int port, registry &ecs, rtype::event::EventListener &listener);
        ~Server();
        void recieve_from_client();
        entity_t get_player_entity_from_connection_address(udp::endpoint);
        entity_t connect_player(udp::endpoint player_endpoint);
        template <typename T>
        void receive_datas(T& structure);
        template <typename T>
        void send_datas(const T& structure);

    private:
        std::array<char, 1024> _buf;
        boost::asio::io_service::work _service;
        udp::endpoint _remote_endpoint;
        std::vector<std::thread> _tpool;
        udp::socket _socket;
        registry &_ecs;
        rtype::event::EventListener &_listener;
};

#endif // SERVER_HPP
