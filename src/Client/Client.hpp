/*
** EPITECH PROJECT, 2023
** R-TYPE
** File description:
** Client.hpp
*/

#ifndef CLIENT_HPP
#define CLIENT_HPP
//#pragma warning(disable: 4668)
//#pragma warning(disable: 4626)
//#pragma warning(disable: 4625)
//#pragma warning(disable: 4820)
//#pragma warning(disable: 5031)
//#pragma warning(disable: 4365)
//#pragma warning(disable: 5027)
//#pragma warning(disable: 4514)
//#pragma warning(disable: 4464)
//#pragma warning(disable: 5026)
//#pragma warning(disable: 4457)
//#pragma warning(disable: 5262)
//#pragma warning(disable: 5204)
//#pragma warning(disable: 4355)
//#pragma warning(disable: 5220)
//#pragma warning(disable: 5039)
#include <iostream>
#include <asio.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <queue>
#include <mutex>
#include "../Ecs/Events.hpp"
#include "../Errors.hpp"
#define MAX_BUF_SIZE 11024

using asio::ip::udp;

struct BaseMessage {
    int16_t id;
    int packet_id;
};

struct SnapshotPosition: public BaseMessage {
    entity_t entity;
    component::Position data;

    SnapshotPosition(): data(0, 0) {};
    SnapshotPosition(int16_t id_, entity_t entity_, component::Position data_, int packet_id_):
    entity(entity_),  data(data_) {
        id = id_;
        packet_id = packet_id_;
    };
};

struct data_struct {
    int id;
    sf::Event event;
    int package_id;

};

struct ConfirmationMessage: public BaseMessage {
};

struct EventMessage: public BaseMessage {
    sf::Event event;
};

enum Stage {
    ONE,
    TWO,
    THREE,
};

class Client {
    typedef int (Client::*messageParserHandle)(std::vector<char>&);
    public:
        Client(std::string ip, int port, std::string _username = "");
        ~Client();
        int run();
        template <typename T>
        void send_to_server(const T& structure);
        // template <typename T>
        // void send_datas(const T& structure);

        // template <typename T>
        // void receive_datas(T& structure);
        void receive_datas();
        void displayTexts();

        void addLevel() { _level++; }
        void addScore() { _score++; }
        void decreaseLives() { _lives--; }
        void increaseLives() { _lives++; }
        void setLevel(int level) { _level = level; }
        bool hasPendingMessages() const;
        int manageEvent();
        void saveHighScore();
        void receive();
        int recieve_position_snapshot_update(std::vector<char> &);
        std::vector<char> recieve_raw_data_from_client();

        void createEnemy(std::pair<float, float> pos, std::pair<float, float> vel, const std::string &path_to_texture, std::pair<float, float> scale, int health, int damage);

    private:
        //Content for network
        EventMessage _send_structure;
        asio::io_context _io_context;
        udp::socket _socket;
        udp::endpoint _server_endpoint;
        std::string _username;
        //Content for network
        SnapshotPosition _recieve_structure;
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
        entity_t _btn_play;
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
        std::map<int16_t, messageParserHandle> _messageParser = {
            {4, &Client::recieve_position_snapshot_update}
        };
        sf::Vector2i _mouse_position;
        sf::Text _mouse_position_text;
};

#endif // CLIENT_HPP
