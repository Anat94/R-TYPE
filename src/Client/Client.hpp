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

using boost::asio::ip::udp;

struct data_struct {
    int id;
    sf::Event::EventType eventType;
};

struct snapshot {
    int id;
    std::string data;
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
        snapshot _receive_structure;
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
                sf::Event::MouseEntered,
                sf::Event::MouseLeft,
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
};

#endif // CLIENT_HPP