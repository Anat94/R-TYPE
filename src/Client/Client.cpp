/*
** EPITECH PROJECT, 2023
** R-TYPE
** File description:
** Client.cpp
*/

#include "Client.hpp"
#include <SFML/Graphics.hpp>
#include "../Ecs/ZipperIterator.hpp"
#include "../Ecs/Events.hpp"

rtype::event::EventListener listener;

auto position_system = [](sparse_array<component::Position> &pos, sparse_array<component::Velocity> &vel, component::DrawableContent& _) {
    for (auto &&[p, v] : zipper<sparse_array<component::Position>, sparse_array<component::Velocity>>(pos, vel)) {
        if (p.has_value() && v.has_value()) {
            p->x += v->_dx;
            p->y += v->_dy;
            if (v->_reset_on_move) {
                v->_dx = 0;
                v->_dy = 0;
            }
        }
    }
};

auto control_system = [](sparse_array<component::Velocity> &vel, sparse_array<component::Controllable> &con, component::DrawableContent& content) {
    int first_ent_idx = 0;
    for (auto &&[v, c] : zipper<sparse_array<component::Velocity>, sparse_array<component::Controllable>>(vel, con)) {
        if (c.has_value() && v.has_value()) {
            if (content.event->type == sf::Event::KeyPressed) {
                if (content.event->key.code == sf::Keyboard::Up)
                    v->_dy = -5;
                if (content.event->key.code == sf::Keyboard::Down)
                    v->_dy = 5;
                if (content.event->key.code == sf::Keyboard::Left)
                    v->_dx = -5;
                if (content.event->key.code == sf::Keyboard::Right)
                    v->_dx = 5;
                if (content.event->key.code == sf::Keyboard::Space)
                    listener.addEvent(new rtype::event::ShootEvent(first_ent_idx, -1));
            }
        }
        first_ent_idx++;
    }
};

auto draw_system = [](sparse_array<component::Drawable> &dra, sparse_array<component::Position> &pos, component::DrawableContent& content) {
    for (auto &&[d, p] : zipper<sparse_array<component::Drawable>, sparse_array<component::Position>>(dra, pos)) {
        if (d.has_value() && p.has_value()) {
            d->shape->setPosition(p->x, p->y);
            content.window->draw(*d->shape);
        }
    }
};

auto collision_system = [](sparse_array<component::Drawable> &dra, sparse_array<component::Position> &pos, component::DrawableContent& _)
{
    int first_ent_idx = 0;
    for (auto &&[d1, p1] : zipper<sparse_array<component::Drawable>, sparse_array<component::Position>>(dra, pos)) {
        if (!d1.has_value() || !p1.has_value()) continue;
        int second_ent_idx = 0;
        for (auto &&[d2, p2] : zipper<sparse_array<component::Drawable>, sparse_array<component::Position>>(dra, pos)) {
            if (first_ent_idx == second_ent_idx)
                continue;
            if ((p1->x <= p2->x &&
                p1->y <= p2->y &&
                (p1->x + 100) >= p2->x &&
                (p1->y + 100) >= p2->y) ||
                (p2->x <= p1->x &&
                p2->y <= p1->y &&
                (p2->x + 100) >= p1->x &&
                (p2->y + 100) >= p1->y)) {
                    rtype::event::CollisionEvent* new_event = new rtype::event::CollisionEvent(second_ent_idx, first_ent_idx);
                if (listener.hasEvent(new_event)) {
                    second_ent_idx++;
                    delete new_event;
                    continue;
                } else
                    listener.addEvent(new_event);
            } else {

            }
            second_ent_idx++;
        }
        first_ent_idx++;
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
    _ecs.register_component<component::HurtsOnCollision>();
    _ecs.register_component<component::Heading>();
    _ecs.register_component<component::Player>();
    //Define the entities
    _player = _ecs.spawn_entity();
    _background = _ecs.spawn_entity();
    _enemy = _ecs.spawn_entity();
    // Define the components for player
    _ecs.add_component(_player, component::Position(10.0f, 10.0f));
    _ecs.add_component(_player, component::Velocity(0.0f, 0.0f, true));
    _ecs.add_component(_player, component::Controllable());
    _ecs.add_component(_player, component::Heading());
    _ecs.add_component(_player, component::Drawable(new sf::RectangleShape({100, 100}), sf::Color::Blue));
    _ecs.add_component(_player, component::Player(100, 20));
    // Define the components for background
    _ecs.add_component(_background, component::Position(10.0f, 10.0f));
    _ecs.add_component(_background, component::Drawable(new sf::RectangleShape({100, 100}), sf::Color::Blue));
    // Define the components for ennemy
    _ecs.add_component(_enemy, component::Position(700.0f, 500.0f));
    _ecs.add_component(_enemy, component::Velocity(0.0f, 0.0f));
    _ecs.add_component(_enemy, component::Drawable(new sf::RectangleShape({100, 100}), sf::Color::Blue));
    _ecs.add_component(_enemy, component::Player(100, 20));

    // Define the window
    _window.create(sf::VideoMode(1920, 1080), "R-Type");
    _window.setFramerateLimit(60);
    listener.addRegistry(_ecs);
    // Define the component
    _ecs.add_system<component::Position, component::Velocity>(position_system);
    _ecs.add_system<component::Velocity, component::Controllable>(control_system);
    _ecs.add_system<component::Drawable, component::Position>(draw_system);
    _ecs.add_system<component::Drawable, component::Position>(collision_system);
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
    _window.draw(_score_text);
    _window.draw(_lives_text);
    _window.draw(_level_text);
}

void Client::manageEvent()
{
    while (listener.popEvent());
    if (_event.type == sf::Event::KeyPressed)
        if (_event.key.code == sf::Keyboard::Escape)
            std::exit(0);

}

int Client::run()
{
    std::cout << "Enter a message to send (Press Ctrl+C to exit):\n";
    while (true) {
        _window.clear();
        _window.pollEvent(_event);
        component::DrawableContent content = component::DrawableContent(_window, _event);
        manageEvent();
        _ecs.run_systems(content);

        // send_datas();
        // receive_datas();
        displayTexts();
        _window.display();

    }
    _window.close();
    return 0;
}