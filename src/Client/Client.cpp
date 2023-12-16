/*
** EPITECH PROJECT, 2023
** R-TYPE
** File description:
** Client.cpp
*/

#include "Client.hpp"
#include <SFML/Graphics.hpp>

auto position_system = [](sparse_array<component::Position> &pos, sparse_array<component::Velocity> &vel, component::DrawableContent& _) {
    for (size_t i = 0; i < pos.size() && i < vel.size(); ++i) {
        if (pos[i] && vel[i]) {
            pos[i]->x += vel[i]->dx;
            pos[i]->y += vel[i]->dy;
            // vel[i]->dx = 0;
            // vel[i]->dy = 0;
        }
    }
};

auto control_system = [](sparse_array<component::Velocity> &vel, sparse_array<component::Controllable> &con, component::DrawableContent& content) {
    for (size_t i = 0; i < con.size() && i < vel.size(); ++i) {
        if (con[i]) {
            if (content.event->type == sf::Event::KeyPressed) {
                if (content.event->key.code == sf::Keyboard::Up)
                    vel[i]->dy = -5;
                if (content.event->key.code == sf::Keyboard::Down)
                    vel[i]->dy = 5;
                if (content.event->key.code == sf::Keyboard::Left)
                    vel[i]->dx = -5;
                if (content.event->key.code == sf::Keyboard::Right)
                    vel[i]->dx = 5;
            }
        }
    }
};

auto draw_system = [](sparse_array<component::Drawable> &dra, sparse_array<component::Position> &pos, component::DrawableContent& content) {
    for (size_t i = 0; i < dra.size() && i < pos.size(); ++i) {
        if (dra[i] && pos[i]) {
            dra[i].value().shape->setPosition(pos[i]->x, pos[i]->y);
            content.window->draw(*dra[i].value().shape);
        }
    }
};

Client::Client(std::string ip, int port)
    : _io_context(),
      _socket(_io_context, udp::endpoint(udp::v4(), 0)),
      _server_endpoint(udp::endpoint(boost::asio::ip::address::from_string(ip), port))
{
    // Define the systems
    _ecs.register_component<component::Position>();
    _ecs.register_component<component::Velocity>();
    _ecs.register_component<component::Drawable>();
    _ecs.register_component<component::Controllable>();
    //Define the entities
    _player = _ecs.spawn_entity();
    _background = _ecs.spawn_entity();
    _enemy = _ecs.spawn_entity();
    _bullet = _ecs.spawn_entity();
    // Define the components for player
    _ecs.add_component(_player, component::Position(10.0f, 10.0f));
    _ecs.add_component(_player, component::Velocity(0.0f, 0.0f));
    _ecs.add_component(_player, component::Controllable());
    _ecs.add_component(_player, component::Drawable(new sf::RectangleShape({100, 100}), sf::Color::Blue));
    // Define the components for background
    _ecs.add_component(_background, component::Position(10.0f, 10.0f));
    _ecs.add_component(_background, component::Drawable(new sf::RectangleShape({100, 100}), sf::Color::Blue));
    // Define the components for ennemy
    _ecs.add_component(_enemy, component::Position(10.0f, 10.0f));
    _ecs.add_component(_enemy, component::Velocity(0.0f, 0.0f));
    _ecs.add_component(_enemy, component::Drawable(new sf::RectangleShape({100, 100}), sf::Color::Blue));
    // Define the components for bullet
    // _ecs.add_component(_bullet, component::Position(10.0f, 10.0f));
    _ecs.add_component(_bullet, component::Velocity(0.0f, 0.0f));
    // Define the window
    _window.create(sf::VideoMode(1920, 1080), "R-Type");
    _window.setFramerateLimit(60);
    // Define the component
    _ecs.add_system<component::Position, component::Velocity>(position_system);
    _ecs.add_system<component::Velocity, component::Controllable>(control_system);
    _ecs.add_system<component::Drawable, component::Position>(draw_system);
    //Define the gameplay
    _score = 0;
    _lives = 3;
    _level = 1;
    _font = sf::Font();
    if (!_font.loadFromFile("src/Client/assets/font.ttf"))
        throw SFMLError("Font not found");
    _score_text = sf::Text("Score: " + std::to_string(_score), _font, 30);
    _score_text.setPosition(10, 10);
    _lives_text = sf::Text("Lives: " + std::to_string(_lives), _font, 30);
    _lives_text.setPosition(1750, 10);
    _level_text = sf::Text("Level: " + std::to_string(_level), _font, 30);
    _level_text.setPosition(950, 10);

}

Client::~Client()
{
    _font.~Font();
}

void Client::send_datas()
{
    std::string message;
    std::getline(std::cin, message);
    _socket.send_to(boost::asio::buffer(message), _server_endpoint);
}

void Client::receive_datas()
{
    char receive_buffer[1024];
    udp::endpoint sender_endpoint;
    size_t bytes_received = _socket.receive_from(boost::asio::buffer(receive_buffer), sender_endpoint);
    receive_buffer[bytes_received] = '\0';
    std::cout << "Received from " << sender_endpoint.address().to_string() << ": " << receive_buffer << std::endl;
}

void Client::displayTexts()
{
    _score_text.setString("Score: " + std::to_string(_score));
    _lives_text.setString("Lives: " + std::to_string(_lives));
    _level_text.setString("Level: " + std::to_string(_level));
}

void Client::manageEvent()
{
    while (_window.pollEvent(_event)) {
        if (_event.type == sf::Event::Closed)
            _window.close();
        if (_event.type == sf::Event::KeyPressed) {
            if (_event.key.code == sf::Keyboard::Escape)
                _window.close();
            if (_event.key.code == sf::Keyboard::Space) {
                _ecs.add_component(_bullet, component::Position(10.0f, 10.0f));
                _ecs.add_component(_bullet, component::Drawable(new sf::RectangleShape({100, 100}), sf::Color::Blue));
            }
        }
    }

}

int Client::run()
{
    std::cout << "Enter a message to send (Press Ctrl+C to exit):\n";
    while (true) {
        _window.clear();
        _window.pollEvent(_event);
        component::DrawableContent content = component::DrawableContent(_window, _event);
        // _ecs.run_systems(content);
        manageEvent();

        // send_datas();
        // receive_datas();
        _window.display();

    }
    _window.close();
    return 0;
}