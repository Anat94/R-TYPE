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
#include "../Ecs/Events.hpp"
#include <SFML/Graphics.hpp>
#include "../Errors.hpp"

using boost::asio::ip::udp;

class Client {
    public:
        Client(std::string ip, int port);
        ~Client();
        int run();
        void send_datas();
        void receive_datas();
        void displayTexts();

        void addLevel() { _level++; }
        void addScore() { _score++; }
        void decreaseLives() { _lives--; }
        void increaseLives() { _lives++; }
        void setLevel(int level) { _level = level; }

        void manageEvent();

    private:
        //Content for network
        boost::asio::io_context _io_context;
        udp::endpoint _server_endpoint;
        udp::socket _socket;
        //Content for ECS
        registry _ecs;
        entity_t _player;
        entity_t _background;
        entity_t _enemy;
        //Content for SFML
        sf::RenderWindow _window;
        sf::Event _event;
        sf::Font _font;
        //Content for gameplay
        int _score;
        int _lives;
        int _level;
        sf::Text _score_text;
        sf::Text _lives_text;
        sf::Text _level_text;
};

#endif // CLIENT_HPP