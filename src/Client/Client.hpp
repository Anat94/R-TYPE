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
#include <vector>
#include "../Ecs/Events.hpp"
#include "../Errors.hpp"
#include <queue>

using boost::asio::ip::udp;

struct data_struct {
    int id;
    sf::Event::EventType eventType;
};

struct snapshot_position {
    int id;
    entity_t entity;
    component::Position data;

    snapshot_position(): data(0, 0) {};
};

struct snapshot_velocity {
    int id;
    sparse_array<component::Velocity> data;
};

struct snapshot_player {
    int id;
    sparse_array<component::Player> data;
};

enum Stage {
    ONE,
    TWO,
    THREE,
};

class Client {
    public:
        Client(std::string ip, int port, std::string _username);
        ~Client();
        int run();
        template <typename T>
        void send_datas(const T& structure);

        template <typename T>
        void receive_datas(T& structure);
        void receive_datas();
        void displayTexts();

        void addLevel() { _level++; }
        void addScore() { _score++; }
        void decreaseLives() { _lives--; }
        void increaseLives() { _lives++; }
        void setLevel(int level) { _level = level; }
        bool hasPendingMessages() const;
        void manageEvent();
        void saveHighScore();
        void receive();

        void createEnemy(std::pair<float, float> pos, std::pair<float, float> vel, const std::string &path_to_texture, std::pair<float, float> scale, int health, int damage);

    private:
        std::string _username;
        //Content for network
        boost::asio::io_context _io_context;
        udp::endpoint _server_endpoint;
        udp::socket _socket;
        data_struct _send_structure;
        snapshot_position _recieve_structure;
        std::array<char, 1024> _receiveBuffer;
        std::vector<sf::Event::EventType> eventsToPrint = {
                sf::Event::Closed,
                sf::Event::Resized,
                sf::Event::LostFocus,
                sf::Event::GainedFocus,
                sf::Event::TextEntered,
                sf::Event::KeyPressed,
                sf::Event::KeyReleased,
                sf::Event::MouseWheelMoved,
                sf::Event::MouseWheelScrolled,
                sf::Event::MouseButtonPressed,
                sf::Event::MouseButtonReleased,
                //sf::Event::MouseMoved,
                // sf::Event::MouseEntered,
                // sf::Event::MouseLeft,
                sf::Event::JoystickButtonPressed,
                sf::Event::JoystickButtonReleased,
                sf::Event::JoystickMoved,
                sf::Event::JoystickConnected,
                sf::Event::JoystickDisconnected,
                sf::Event::TouchBegan,
                sf::Event::TouchMoved,
                sf::Event::TouchEnded,
                sf::Event::SensorChanged
            };
        //Content for ECS
        registry _ecs;
        entity_t _player;
        entity_t _background;
        entity_t _enemy;
        //Content for SFML
        sf::RenderWindow _window;
        sf::Event _event;
        sf::Font _font;
        sf::Music _music;
        //Content for gameplay
        int _score;
        int _lives;
        int _level;
        sf::Text _score_text;
        sf::Text _lives_text;
        sf::Text _level_text;
        Stage _stage;
        // content for enemys
        std::queue<entity_t> _enemiesQueue;
};

#endif // CLIENT_HPP