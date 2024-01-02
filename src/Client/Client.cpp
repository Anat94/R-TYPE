/*
** EPITECH PROJECT, 2023
** R-TYPE
** File description:
** Client.cpp
*/
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
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "../Ecs/ZipperIterator.hpp"
#include "Client.hpp"
#include "../Ecs/Systems/DrawSystem.hpp"
#include "../Ecs/Systems/RotationSystem.hpp"
#include "../Ecs/Systems/ControlSystem.hpp"
#include "../Ecs/Systems/ScaleSystem.hpp"
#include "../Ecs/Systems/ButtonSystem.hpp"
#include "../KeyEventMapping.hpp"

bool can_read = true;
std::mutex mtx;
EventListener listener;

std::vector<char> Client::recieve_raw_data_from_client()
{
    std::vector<char> receivedData(MAX_BUF_SIZE);
    size_t bytesRead = _socket.receive_from(asio::buffer(receivedData), _server_endpoint);

    receivedData.resize(bytesRead);
    return receivedData;
}

int Client::recieve_position_snapshot_update(std::vector<char> &server_msg)
{
    if (server_msg.size() < sizeof(SnapshotPosition))
        return -1;
    SnapshotPosition *snapshot = reinterpret_cast<SnapshotPosition *>(server_msg.data());
    sparse_array<component::Position> &pos = _ecs.get_components<component::Position>();
    sparse_array<component::ServerEntity> &servEntities = _ecs.get_components<component::ServerEntity>();
    while (!can_read)
        continue;
    try {
        entity_t real_entity = snapshot->entity + 2;
        // for (size_t j = 0; j < servEntities.size(); j++) {
        //     std::cout << "j is: " << j << std::endl;
        //     real_entity = (servEntities[j].has_value() && servEntities[j].value().entity == snapshot->entity) ? servEntities[j].value().entity : real_entity;
        // }
        if (real_entity > 0 && pos[real_entity].has_value()) {
            std::cout << "UPDATED PLAYER\n";
            std::cout << snapshot->data.x << std::endl;
            pos[real_entity]->x = snapshot->data.x;
            pos[real_entity]->y = snapshot->data.y;
        } else {
            std::cout << "CREATED PLAYER\n";
            entity_t new_player = _ecs.spawn_entity();
            std::cout << _recieve_structure.data.x << std::endl;
            _ecs.add_component(new_player, component::Position(snapshot->data.x,  snapshot->data.y));
            _ecs.add_component(new_player, component::Velocity(0.0f, 0.0f));
            _ecs.add_component(new_player, component::ResetOnMove());
            _ecs.add_component(new_player, component::Heading());
            _ecs.add_component(new_player, component::Drawable("src/Client/assets/ship.png"));
            _ecs.add_component(new_player, component::Scale(0.1f));
            _ecs.add_component(new_player, component::Rotation(90));
            _ecs.add_component(new_player, component::Controllable());
            _ecs.add_component(new_player, component::ServerEntity(snapshot->entity));
            std::cout << snapshot->data.x << ", " << snapshot->data.y << std::endl;
        }
    } catch (const std::exception &ex) {
        std::cout << ex.what() << std::endl;
    }
    return snapshot->packet_id;
}

void Client::receive()
{
    std::vector<char> server_msg = recieve_raw_data_from_client();
    std::cout << "recieved raw" << std::endl;
    if (server_msg.size() < sizeof(BaseMessage))
        return;
    BaseMessage *baseMsg = reinterpret_cast<BaseMessage *>(server_msg.data());

    if (_messageParser.find(baseMsg->id) == _messageParser.end())
        throw ArgumentError("ERROR: Invalid event recieved: " + std::to_string(baseMsg->id) + ".");
    int packet_id = (this->*_messageParser[baseMsg->id])(server_msg);
    ConfirmationMessage to_send;
    to_send.id = 5;
    to_send.packet_id = packet_id;
    send_to_server<ConfirmationMessage>(to_send);
    receive();
}

Client::Client(std::string ip, int port, std::string username)
    : _io_context(),
      _socket(_io_context, udp::endpoint(udp::v4(), 0)),
      _server_endpoint(udp::endpoint(asio::ip::make_address(ip), port)),
      _username(username)
{
    _send_structure.id = 2;
    send_to_server(_send_structure);
    _ecs.register_component<component::Scale>();
    _ecs.register_component<component::Score>();
    _ecs.register_component<component::Damage>();
    _ecs.register_component<component::Health>();
    _ecs.register_component<component::Pierce>();
    _ecs.register_component<component::Hitbox>();
    _ecs.register_component<component::Heading>();
    _ecs.register_component<component::Position>();
    _ecs.register_component<component::Velocity>();
    _ecs.register_component<component::Drawable>();
    _ecs.register_component<component::Rotation>();
    _ecs.register_component<component::ResetOnMove>();
    _ecs.register_component<component::ServerEntity>();
    _ecs.register_component<component::Controllable>();
    _ecs.register_component<component::HurtsOnCollision>();
    _background = _ecs.spawn_entity();
    _btn_play = _ecs.spawn_entity();
    _ecs.add_component(_background, component::Position(0.0f, 0.0f));
    _ecs.add_component(_background, component::Drawable("src/Client/assets/background.jpg"));
    if (!_music.openFromFile("src/Client/assets/game_music.ogg"))
        throw SFMLError("Music not found");

    _window.create(sf::VideoMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height), "R-Type");
    _window.setFramerateLimit(60);
    listener.addRegistry(_ecs);
    ControlSystem *ctrl_sys = new ControlSystem(&listener, &_event);
    _ecs.add_system<component::Velocity, component::Controllable>(*ctrl_sys);
    ScaleSystem *sca_sys = new ScaleSystem;
    _ecs.add_system<component::Drawable, component::Scale>(*sca_sys);
    RotationSystem *rot_sys = new RotationSystem;
    _ecs.add_system<component::Drawable, component::Rotation>(*rot_sys);
    DrawSystem *draw_sys = new DrawSystem(&_window);
    _ecs.add_system<component::Drawable, component::Position>(*draw_sys);
    _score = 0;
    _lives = 0;
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
    _mouse_position_text = sf::Text("Mouse: " + std::to_string(_mouse_position.x) + ", " + std::to_string(_mouse_position.y), _font, 20);
    _mouse_position_text.setPosition(10, 970);
}

Client::~Client()
{
    _font.~Font();
    _music.stop();
}

void Client::createEnemy(std::pair<float, float> pos, std::pair<float, float> vel, const std::string &path_to_texture, std::pair<float, float> scale, int health, int damage) {
    entity_t _newEnemy = _ecs.spawn_entity();

    _ecs.add_component(_newEnemy, component::Position(pos.first, pos.second));
    _ecs.add_component(_newEnemy, component::Velocity(vel.first, vel.second));
    _ecs.add_component(_newEnemy, component::Drawable(path_to_texture));
    _ecs.add_component(_newEnemy, component::Health(health));

    _enemiesQueue.push(_newEnemy);
}

template <typename T>
void Client::send_to_server(const T& structure) {
    _socket.send_to(asio::buffer(&structure, sizeof(structure)), _server_endpoint);
}

void Client::receive_datas() {
    std::cout << "START RECIEVE";
    _socket.receive_from(asio::buffer(&_recieve_structure, sizeof(_recieve_structure)), _server_endpoint);
    std::cout << "RECIEVED\n";
    receive_datas();
}

void Client::displayTexts()
{
    _window.draw(_score_text);
    _window.draw(_lives_text);
    _window.draw(_level_text);
    _window.draw(_mouse_position_text);
}

int Client::manageEvent()
{
    while (_window.pollEvent(_event)) {
        if (_event.type == sf::Event::Closed) {
            _send_structure.id = 3;
            send_to_server<EventMessage>(_send_structure);
            return 1;
        }
        if (std::find(eventsToPrint.begin(), eventsToPrint.end(), _event.type) != eventsToPrint.end()) {
            _send_structure.id = 1;
            if (SFMLKeys.find(_event.key.code) != SFMLKeys.end())
                _send_structure.event = KeyIds[SFMLKeys[_event.key.code]];
            else
                _send_structure.event = -1;
            send_to_server(_send_structure);
            _event = _event;
            return 0;
        }
        if (_event.type == sf::Event::KeyPressed)
            if (_event.key.code == sf::Keyboard::Escape) {
                saveHighScore();
                return 1;
            }
    }
    return 0;
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
    _lives = 0; // ((player1_h.has_value()) ? (player1_h->_health) : (0));
    _score = 0; // ((player1_s.has_value()) ? (player1_s->_score) : (0));
    _score_text.setString("Score: " + std::to_string(_score));
    _lives_text.setString("Health: " + std::to_string(_lives));
    _lives_text.setPosition(1750, 10);
    while (true) {
        _mouse_position = sf::Mouse::getPosition(_window);
        _mouse_position_text.setString("Mouse: " + std::to_string(_mouse_position.x) + ", " + std::to_string(_mouse_position.y));
        // auto &player1_h = _ecs.get_components<component::Health>()[_player];
        // auto &player1_s = _ecs.get_components<component::Score>()[_player];
        _window.clear();
        if (manageEvent())
            break;
        while (listener.popEvent());
        std::cout << "Here" << std::endl;
        _ecs.run_systems();
        std::cout << " 1 tour done" << std::endl;
        displayTexts();
        _window.display();
    }
    saveHighScore();
    _window.close();
    return 0;
}
