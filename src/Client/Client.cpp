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
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

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
                    v->_dy = -30;
                if (content.event->key.code == sf::Keyboard::Down)
                    v->_dy = 30;
                if (content.event->key.code == sf::Keyboard::Left)
                    v->_dx = -30;
                if (content.event->key.code == sf::Keyboard::Right)
                    v->_dx = 30;
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
            d->set();
            d->_sprite.setPosition(p->x, p->y);
            content.window->draw(d->_sprite);
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

Client::Client(std::string ip, int port, std::string username)
    : _io_context(),
      _socket(_io_context, udp::endpoint(udp::v4(), 0)),
      _server_endpoint(udp::endpoint(boost::asio::ip::address::from_string(ip), port)),
      _username(username)
{
    // Define the systems
    _ecs.register_component<component::Position>();
    _ecs.register_component<component::Velocity>();
    _ecs.register_component<component::Drawable>();
    _ecs.register_component<component::Controllable>();
    _ecs.register_component<component::HurtsOnCollision>();
    _ecs.register_component<component::Heading>();
    _ecs.register_component<component::Player>();
    _ecs.register_component<component::PlayMusic>();
    //Define the entities
    _background = _ecs.spawn_entity();
    _player = _ecs.spawn_entity();
    //_enemy = _ecs.spawn_entity();
    // Define the components for background
    _ecs.add_component(_background, component::Position(0.0f, 10.0f));
    _ecs.add_component(_background, component::Drawable("src/Client/assets/background.jpg", {1., 1.}));
    // Define the components for player
    _ecs.add_component(_player, component::Position(150.0f, 400.0f));
    _ecs.add_component(_player, component::Velocity(0.0f, 0.0f, true));
    _ecs.add_component(_player, component::Controllable());
    _ecs.add_component(_player, component::Heading());
    _ecs.add_component(_player, component::Drawable("src/Client/assets/ship.png", {0.1, 0.1}, 90));
    _ecs.add_component(_player, component::Player(100, 20));
    // sf::Music music;
    if (!_music.openFromFile("src/Client/assets/game_music.ogg"))
        throw SFMLError("Music not found");




    // Define the components for ennemy


    createEnemy(std::pair(700.0f, 500.0f), std::pair(0.0f, 0.0f), std::string("src/Client/assets/ennemy.png"),  std::pair(0.1, 0.1), 100, 20);
    createEnemy(std::pair(1000.0f, 500.0f), std::pair(0.0f, 0.0f), std::string("src/Client/assets/ennemy.png"),  std::pair(0.1, 0.1), 100, 20);



    // Define the window
    _window.create(sf::VideoMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height), "R-Type");
    _window.setFramerateLimit(60);
    listener.addRegistry(_ecs);
    // Define the component
    _ecs.add_system<component::Position, component::Velocity>(position_system);
    _ecs.add_system<component::Velocity, component::Controllable>(control_system);
    _ecs.add_system<component::Drawable, component::Position>(draw_system);
    _ecs.add_system<component::Drawable, component::Position>(collision_system);
    //Define the gameplay
    auto &player1 = _ecs.get_components<component::Player>()[_player];
    _score = 0;
    _lives = player1.value()._health;
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

void Client::createEnemy(std::pair<float, float> pos, std::pair<float, float> vel, const std::string &path_to_texture, std::pair<float, float> scale, int health, int damage) {
    entity_t _newEnemy = _ecs.spawn_entity();

    _ecs.add_component(_newEnemy,  component::Position(pos.first, pos.second));
    _ecs.add_component(_newEnemy, component::Velocity(vel.first, vel.second));
    _ecs.add_component(_newEnemy, component::Drawable(path_to_texture, {scale.first, scale.second}));
    _ecs.add_component(_newEnemy, component::Player(health, damage));

    _enemiesQueue.push(_newEnemy);
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

void Client::saveHighScore()
{
    std::ofstream file;
    file.open("./db.txt");
    if (file.is_open()) {
        printf("Score: %d\n", _score);
        file << _score;
        file.close();
    }
    printf("NONScore: %d\n", _score);

}

int Client::run()
{
    std::cout << "Enter a message to send (Press Ctrl+C to exit):\n";
    _music.play();
    _music.setLoop(true);
    while (true) {
        auto &player1 = _ecs.get_components<component::Player>()[_player];
        _lives = player1.value()._health;
        _score = player1.value()._xp;
        _score_text.setString("Score: " + std::to_string(_score));
        _lives_text.setString("Score: " + std::to_string(_lives));
        _lives_text.setPosition(1750, 10);
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
    saveHighScore();
    _window.close();
}