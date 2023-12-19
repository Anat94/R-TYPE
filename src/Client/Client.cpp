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

bool can_read = true;
std::mutex mtx;
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

// auto control_system = [](sparse_array<component::Velocity> &vel, sparse_array<component::Controllable> &con, component::DrawableContent& content) {
//     int first_ent_idx = 0;
    // for (auto &&[v, c] : zipper<sparse_array<component::Velocity>, sparse_array<component::Controllable>>(vel, con)) {
    //     if (c.has_value() && v.has_value()) {
    //         if (content.event->type == sf::Event::KeyPressed) {
    //             if (content.event->key.code == sf::Keyboard::Up)
    //                 v->_dy = -30;
    //             if (content.event->key.code == sf::Keyboard::Down)
    //                 v->_dy = 30;
    //             if (content.event->key.code == sf::Keyboard::Left)
    //                 v->_dx = -30;
    //             if (content.event->key.code == sf::Keyboard::Right)
    //                 v->_dx = 30;
    //             if (content.event->key.code == sf::Keyboard::Space)
    //                 listener.addEvent(new rtype::event::ShootEvent(first_ent_idx, -1));
    //         }
    //     }
    //     first_ent_idx++;
    // }
// };

auto draw_system = [](sparse_array<component::Drawable> &dra, sparse_array<component::Position> &pos, component::DrawableContent& content) {
    can_read = false;
    std::lock_guard<std::mutex> lock(mtx);
    for (auto &&[d, p] : zipper<sparse_array<component::Drawable>, sparse_array<component::Position>>(dra, pos)) {
        if (d.has_value() && p.has_value()) {
            std::cout << "HAS VALUE\n";
            d->set();
            d->_sprite.setPosition(p->x, p->y);
            content.window->draw(d->_sprite);
        }
    }
    std::cout << "HAS VALUE\n";
    can_read = true;
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

bool Client::hasPendingMessages() const {
    return _socket.available() > 0;
}

void Client::receive() {
    _socket.receive_from(boost::asio::buffer(&_recieve_structure, sizeof(_recieve_structure)), _server_endpoint);
    std::cout << "RECIEVED\n";
    sparse_array<component::Position> pos = _ecs.get_components<component::Position>();
    sparse_array<component::ServerEntity> servEntities = _ecs.get_components<component::ServerEntity>();
    while (!can_read)
        continue;
    try {
        // for (size_t j = 0; j < servEntities.size(); j++) {
        //     std::cout << "j is: " << j << servEntities[j].value().entity << std::endl;
        //     real_entity = (servEntities[j].has_value() && servEntities[j].value().entity == _recieve_structure.entity) ? servEntities[j].value().entity : real_entity;
        // }
        entity_t real_entity = _recieve_structure.entity + 1;
        if (real_entity > 0 && pos[real_entity].has_value()) {
            std::cout << "UPDATED PLAYER\n";
            std::cout << _recieve_structure.data.x << std::endl;
            pos[real_entity].value().x = _recieve_structure.data.x;
            pos[real_entity].value().y = _recieve_structure.data.y;
        } else {
            // std::lock_guard<std::mutex> lock(mtx);
            std::cout << "CREATED PLAYER\n";
            entity_t new_player = _ecs.spawn_entity();
            // std::cout << _recieve_structure.entity << std::endl;
            // std::cout << new_player << std::endl;
            std::cout << _recieve_structure.data.x << std::endl;
            _ecs.add_component(new_player, component::Position(_recieve_structure.data.x,  _recieve_structure.data.y));
            _ecs.add_component(new_player, component::Velocity(0.0f, 0.0f, true));
            // _ecs.add_component(new_player, component::Controllable());
            _ecs.add_component(new_player, component::Heading());
            _ecs.add_component(new_player, component::Drawable("src/Client/assets/ship.png", {0.1, 0.1}, 90));
            // _ecs.add_component(new_player, component::Player(100, 20));
            _ecs.add_component(new_player, component::ServerEntity(_recieve_structure.entity));
            std::cout << "FINISHED CREATING\n";
        }
    } catch (std::exception ex) {
        std::cout << ex.what() << std::endl;
    }
    _send_structure.id = 5;
    _send_structure.package_id = _recieve_structure.package_id;
    send_datas(_send_structure);
    receive();
}

Client::Client(std::string ip, int port, std::string username)
    : _io_context(),
      _socket(_io_context, udp::endpoint(udp::v4(), 0)),
      _server_endpoint(udp::endpoint(boost::asio::ip::address::from_string(ip), port)),
      _username(username)
{
    _send_structure.id = 2;
    send_datas(_send_structure);
    // Define the systems
    _ecs.register_component<component::Position>();
    _ecs.register_component<component::Velocity>();
    _ecs.register_component<component::Drawable>();
    // _ecs.register_component<component::Controllable>();
    _ecs.register_component<component::HurtsOnCollision>();
    _ecs.register_component<component::Heading>();
    _ecs.register_component<component::Player>();
    _ecs.register_component<component::PlayMusic>();
    _ecs.register_component<component::ServerEntity>();
    //Define the entities
    _background = _ecs.spawn_entity();
    // _player = _ecs.spawn_entity();
    // _enemy = _ecs.spawn_entity();
    // Define the components for background
    _ecs.add_component(_background, component::Position(0.0f, 10.0f));
    _ecs.add_component(_background, component::Drawable("src/Client/assets/background.jpg", {1., 1.}));
    // Define the components for player
    // _ecs.add_component(_player, component::Position(150.0f, 400.0f));
    // _ecs.add_component(_player, component::Velocity(0.0f, 0.0f, true));
    // _ecs.add_component(_player, component::Controllable());
    // _ecs.add_component(_player, component::Heading());
    // _ecs.add_component(_player, component::Drawable("src/Client/assets/ship.png", {0.1, 0.1}, 90));
    // _ecs.add_component(_player, component::Player(100, 20));
    sf::Music music;
    if (!_music.openFromFile("src/Client/assets/game_music.ogg"))
        throw SFMLError("Music not found");

    // Define the components for ennemy
    // _ecs.add_component(_enemy,  component::Position(700.0f, 500.0f));
    // _ecs.add_component(_enemy, component::Velocity(0.0f, 0.0f));
    // _ecs.add_component(_enemy, component::Drawable("src/Client/assets/ennemy.png", {0.1, 0.1}));
    // _ecs.add_component(_enemy, component::Player(100, 20));

    // Define the window
    _window.create(sf::VideoMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height), "R-Type");
    _window.setFramerateLimit(60);
    listener.addRegistry(_ecs);
    // Define the component
    _ecs.add_system<component::Position, component::Velocity>(position_system);
    // _ecs.add_system<component::Velocity, component::Controllable>(control_system);
    _ecs.add_system<component::Drawable, component::Position>(draw_system);
    _ecs.add_system<component::Drawable, component::Position>(collision_system);
    //Define the gameplay
    // auto &player1 = _ecs.get_components<component::Player>()[_player];
    _score = 0;
    _lives = 0; // player1.value()._health;
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

template <typename T>
void Client::send_datas(const T& structure) {
    _socket.send_to(boost::asio::buffer(&structure, sizeof(structure)), _server_endpoint);
}

void Client::receive_datas() {
    std::cout << "START RECIEVE";
    _socket.receive_from(boost::asio::buffer(&_recieve_structure, sizeof(_recieve_structure)), _server_endpoint);
    std::cout << "RECIEVED\n";
    receive_datas();
}

void Client::displayTexts()
{
    _window.draw(_score_text);
    _window.draw(_lives_text);
    _window.draw(_level_text);
}

void Client::manageEvent()
{
    sf::Event evt;
    while (_window.pollEvent(evt)) {
        if (evt.type == sf::Event::Closed) {
            _send_structure.id = 3;
            send_datas<data_struct>(_send_structure);
            _window.close();
            std::exit(0);
        }
        if (std::find(eventsToPrint.begin(), eventsToPrint.end(), evt.type) != eventsToPrint.end()) {
            _send_structure.id = 1;
            _send_structure.event = evt;
            send_datas<data_struct>(_send_structure);
            _event = evt;
            return;
        }
        if (evt.type == sf::Event::KeyPressed)
            if (evt.key.code == sf::Keyboard::Escape) {
                saveHighScore();
                _window.close();
                std::exit(0);
            }
    }
}

void Client::saveHighScore()
{
    try {

        std::ifstream myFile("db.txt");
        if (!myFile.is_open()) {
            throw std::runtime_error("Failed to open file for writing.");
        }
        int lastScore = 0;
        std::string firstLine;
        if (std::getline(myFile, firstLine)) {
            std::cout << firstLine << std::endl;
            size_t colonPos = firstLine.find(':');
            std::string score = firstLine.substr(colonPos + 1);
            score.erase(0, score.find_first_not_of(" "));
            score.erase(score.find_last_not_of(" ") + 1);
            lastScore = std::stoi(score);
        } else {
            throw std::runtime_error("Failed to read the first line from the file.");
        }

        myFile.close();
        std::fstream file("db.txt");
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file for writing.");
        }
        if (lastScore <= _score)
            file << _username << ": "<< _score << std::endl;

        if (file.fail()) {
            throw std::runtime_error("Failed to write to the file.");
        }
    } catch (const std::exception& e) {
        std::cout << "An error occurred: " << e.what() << std::endl;
    }
}

int Client::run()
{
    _music.play();
    _music.setLoop(true);
    std::thread receiveThread(&Client::receive, this);
    _music.setVolume(25);
    while (true) {
        // auto &player1 = _ecs.get_components<component::Player>()[_player];
        _lives = 1;//player1.value()._health;
        _score = 0; //player1.value()._xp;
        _score_text.setString("Score: " + std::to_string(_score));
        _lives_text.setString("Score: " + std::to_string(_lives));
        _lives_text.setPosition(1750, 10);
        _window.clear();
        manageEvent();
        component::DrawableContent content = component::DrawableContent(_window, _event);
        while (listener.popEvent());
        _ecs.run_systems(content);
        // send_datas();
        // receive_datas();
        displayTexts();
        _window.display();
    }
    saveHighScore();
    _window.close();
}