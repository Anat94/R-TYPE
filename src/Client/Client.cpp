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
#include "../Ecs/Systems/SFMLAnimatedDrawSystem.hpp"
#include "../Ecs/Systems/SFMLTextDrawSystem.hpp"
#include "../Ecs/Systems/SFMLDrawSystem.hpp"
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

int Client::recieve_high_score(std::vector<char> &server_msg)
{
    if (server_msg.size() < sizeof(HighScoreMessage))
        return -1;
    HighScoreMessage *snapshot = reinterpret_cast<HighScoreMessage *>(server_msg.data());
    while (!can_read)
        continue;
    _highScoreDisplay.name1.setString(snapshot->name1);
    _highScoreDisplay.name2.setString(snapshot->name2);
    _highScoreDisplay.name3.setString(snapshot->name3);
    _highScoreDisplay.score1.setString(std::to_string(snapshot->score1));
    _highScoreDisplay.score2.setString(std::to_string(snapshot->score2));
    _highScoreDisplay.score3.setString(std::to_string(snapshot->score3));
    return snapshot->packet_id;

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
            // std::cout << "UPDATED PLAYER\n";
            // std::cout << snapshot->data.x << std::endl;
            pos[real_entity]->x = snapshot->data.x;
            pos[real_entity]->y = snapshot->data.y;
        } else {
            // std::cout << "CREATED PLAYER\n";
            entity_t new_player = _ecs.spawn_entity();
            // std::cout << _recieve_structure.data.x << std::endl;
            _ecs.add_component(new_player, component::Position(snapshot->data.x,  snapshot->data.y));
            _ecs.add_component(new_player, component::Velocity(0.0f, 0.0f));
            _ecs.add_component(new_player, component::ResetOnMove());
            _ecs.add_component(new_player, component::Heading());
            _ecs.add_component(new_player, component::Scale(0.1f));
            _ecs.add_component(new_player, component::Rotation(90));
            _ecs.add_component(new_player, component::Controllable());
            _ecs.add_component(new_player, component::Clickable());
            _ecs.add_component(new_player, component::Hitbox(component::Position(snapshot->data.x,  snapshot->data.y), component::Position(snapshot->data.x + 32,  snapshot->data.y + 32)));
            _ecs.add_component(new_player, component::ServerEntity(snapshot->entity));
            // std::cout << snapshot->data.x << ", " << snapshot->data.y << std::endl;
        }
    } catch (const std::exception &ex) {
        std::cout << ex.what() << std::endl;
    }
    return snapshot->packet_id;
}

int Client::recieve_login_response(std::vector<char> &server_msg)
{
    if (server_msg.size() < sizeof(LoginResponse))
        return -1;
    LoginResponse *login = reinterpret_cast<LoginResponse *>(server_msg.data());
    if (login->response == true && login->logintype == 1) {
        std::cout << "Connected" << std::endl;
        // _state = GAME;
        return login->packet_id;
    } else if (login->response == false  && login->logintype == 1) {
        std::cout << "Wrong credentials" << std::endl;
        // _state = MENU;
        return login->packet_id;
    } else if (login->response == true && login->logintype == 0) {
        std::cout << "Registered" << std::endl;
        // _state = GAME;
        return login->packet_id;
    } else if (login->response == false && login->logintype == 0) {
        std::cout << "An error occured whil registring" << std::endl;
        // _state = GAME;
        return login->packet_id;
    }
    return -1;
}

int Client::recieve_drawable_snapshot_update(std::vector<char> &server_msg)
{
    if (server_msg.size() < sizeof(DrawableSnapshot))
        return -1;

    // std::cout << server_msg.data() << ", " << sizeof(DrawableSnapshot) << std::endl;
    DrawableSnapshot *snapshot = reinterpret_cast<DrawableSnapshot *>(server_msg.data());
    // std::cout << snapshot->data << std::endl;
    sparse_array<component::Drawable> &drawables = _ecs.get_components<component::Drawable>();
    sparse_array<component::ServerEntity> &servEntities = _ecs.get_components<component::ServerEntity>();
    while (!can_read)
        continue;
    try {
        entity_t real_entity = snapshot->entity + 2;
        // for (size_t j = 0; j < servEntities.size(); j++) {
        //     std::cout << "j is: " << j << std::endl;
        //     real_entity = (servEntities[j].has_value() && servEntities[j].value().entity == snapshot->entity) ? servEntities[j].value().entity : real_entity;
        // }
        if (real_entity > 0 && drawables[real_entity].has_value()) {
            // std::cout << "UPDATED PLAYER SPRITE\n";
            drawables[real_entity]->_path = std::string(snapshot->data);
        } else {
            // std::cout << "CREATED PLAYER SPRITE\n";
            // std::cout << _recieve_structure.data.x << std::endl;
            _ecs.add_component(real_entity, component::Drawable(std::string(snapshot->data)));
        }
    } catch (const std::exception &ex) {
        std::cout << ex.what() << std::endl;
    }
    return snapshot->packet_id;
}

void Client::receive()
{
    std::vector<char> server_msg = recieve_raw_data_from_client();
    // std::cout << "recieved raw" << std::endl;
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
    _ecs.register_component<component::Text>();
    _ecs.register_component<component::Scale>();
    _ecs.register_component<component::Score>();
    _ecs.register_component<component::Damage>();
    _ecs.register_component<component::Health>();
    _ecs.register_component<component::Hitbox>();
    _ecs.register_component<component::Pierce>();
    _ecs.register_component<component::Heading>();
    _ecs.register_component<component::Drawable>();
    _ecs.register_component<component::Position>();
    _ecs.register_component<component::Rotation>();
    _ecs.register_component<component::Velocity>();
    _ecs.register_component<component::Clickable>();
    _ecs.register_component<component::Controllable>();
    _ecs.register_component<component::ResetOnMove>();
    _ecs.register_component<component::ServerEntity>();
    _ecs.register_component<component::AnimatedDrawable>();
    _ecs.register_component<component::HurtsOnCollision>();
    _background = _ecs.spawn_entity();
    _btn_play = _ecs.spawn_entity();
    _ecs.add_component(_background, component::Position(0.0f, 0.0f));
    _ecs.add_component(_background, component::Drawable("./assets/background.jpg"));
    if (!_music.openFromFile("./assets/game_music.ogg"))
        throw SFMLError("Music not found");

    _window.create(sf::VideoMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height), "R-Type");
    _window.setFramerateLimit(60);
    listener.addRegistry(_ecs);
    SFMLDrawSystem *draw_sys = new SFMLDrawSystem(&_window, &_mouse_position);
    _ecs.add_system<component::Drawable, component::Position, component::Clickable, component::Hitbox>(*draw_sys);
    // SFMLAnimatedDrawSystem *tmp_draw_sys = new SFMLAnimatedDrawSystem(&_window, &_mouse_position);
    // _ecs.add_system<component::AnimatedDrawable, component::Position, component::Scale, component::Rotation>(*tmp_draw_sys);
    // SFMLTextDrawSystem *tmp_text_draw_sys = new SFMLTextDrawSystem(&_window);
    // _ecs.add_system<component::Text, component::Position>(*tmp_text_draw_sys);
    // entity_t tmp_text = _ecs.spawn_entity();
    // _ecs.add_component(tmp_text, component::Text("my text to print"));
    // _ecs.add_component(tmp_text, component::Position(100.0f, 550.0f));
    // entity_t tmp_text_2 = _ecs.spawn_entity();
    // _ecs.add_component(tmp_text_2, component::Text("my second text to print"));
    // _ecs.add_component(tmp_text_2, component::Position(200.0f, 600.0f));
    // _player = _ecs.spawn_entity();
    // _ecs.add_component(_player, component::Position(100.0f, 600.0f));
    // _ecs.add_component(_player, component::Scale(5.0f));
    // _ecs.add_component(_player, component::AnimatedDrawable("temp/assets/textures/sprites/r-typesheet42.gif", {5, 1}, {32, 14}, {1, 0}, {1, 3}, {0, 0}));
    // _enemy = _ecs.spawn_entity();
    // _ecs.add_component(_enemy, component::Position(200.0f, 300.0f));
    // _ecs.add_component(_enemy, component::Scale(5.0f));
    // _ecs.add_component(_enemy, component::AnimatedDrawable("temp/assets/textures/sprites/r-typesheet42.gif", {5, 1}, {32, 14}, {1, 0}, {1, 20}, {0, 0}));
    // auto &tmp = _ecs.get_components<component::AnimatedDrawable>()[_enemy];
    // tmp->addAnimation("idle", {0, 4}, false);
    // tmp->addAnimation("move up", {2, 4}, false);
    // tmp->addAnimation("move down", {2, 0}, true);
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
    _highScoreDisplay.name1 = sf::Text("Name1", _font, 30);
    _highScoreDisplay.name2 = sf::Text("Name2", _font, 30);
    _highScoreDisplay.name3 = sf::Text("Name3", _font, 30);
    _highScoreDisplay.score1 = sf::Text("Score1", _font, 30);
    _highScoreDisplay.score2 = sf::Text("Score2", _font, 30);
    _highScoreDisplay.score3 = sf::Text("Score3", _font, 30);
    _highScoreDisplay.title = sf::Text("Score Board", _font, 60);
    _highScoreDisplay.trophy1.sprite = sf::Sprite();
    _highScoreDisplay.trophy1.texture.loadFromFile("./assets/trophy.png");
    _highScoreDisplay.trophy1.sprite.setTexture(_highScoreDisplay.trophy1.texture);
    _highScoreDisplay.trophy1.sprite.setPosition(150, 200);
    _highScoreDisplay.trophy2.sprite = sf::Sprite();
    _highScoreDisplay.trophy2.texture.loadFromFile("./assets/trophy.png");
    _highScoreDisplay.trophy2.sprite.setTexture(_highScoreDisplay.trophy2.texture);
    _highScoreDisplay.trophy2.sprite.setPosition(1250, 200);
    _highScoreDisplay.name1.setPosition(750, 400);
    _highScoreDisplay.name2.setPosition(750, 500);
    _highScoreDisplay.name3.setPosition(750, 600);
    _highScoreDisplay.score1.setPosition(1050, 400);
    _highScoreDisplay.score2.setPosition(1050, 500);
    _highScoreDisplay.score3.setPosition(1050, 600);
    _highScoreDisplay.title.setPosition(750, 200);
    _state = INGAME;
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
    // std::cout << "START RECIEVE";
    _socket.receive_from(asio::buffer(&_recieve_structure, sizeof(_recieve_structure)), _server_endpoint);
    // std::cout << "RECIEVED\n";
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
        if (_event.type == sf::Event::KeyPressed) {
            if (_event.key.code == sf::Keyboard::Escape) {
                _state = (_state == INGAME) ? (INGAMEMENU) : (INGAME);
                return 0;
            }
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
    }
    return 0;
}

void Client::displayScoreBoardMenu()
{
    _window.draw(_highScoreDisplay.trophy1.sprite);
    _window.draw(_highScoreDisplay.title);
    _window.draw(_highScoreDisplay.trophy2.sprite);
    _window.draw(_highScoreDisplay.name1);
    _window.draw(_highScoreDisplay.name2);
    _window.draw(_highScoreDisplay.name3);
    _window.draw(_highScoreDisplay.score1);
    _window.draw(_highScoreDisplay.score2);
    _window.draw(_highScoreDisplay.score3);
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
    LoginMessage login(6, "test", "test", 0, _packet_id); // 0 == signup & 1 == signin
    _packet_id += 1;
    send_to_server<LoginMessage>(login);
    while (true) {
        _mouse_position = sf::Mouse::getPosition(_window);
        _window.clear();
        if (manageEvent())
            break;
        if (_state == INGAMEMENU)
            displayScoreBoardMenu();
        else if (_state == INGAME) {
            _mouse_position_text.setString("Mouse: " + std::to_string(_mouse_position.x) + ", " + std::to_string(_mouse_position.y));
            while (listener.popEvent());
            _ecs.run_systems();
            displayTexts();
        }
        _window.display();
    }
    _window.close();
    return 0;
}
