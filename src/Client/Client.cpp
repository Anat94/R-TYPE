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
#include "../Ecs/Systems/SFMLDrawSystem.hpp"
#include "../Ecs/Systems/RotationSystem.hpp"
#include "../Ecs/Systems/ControlSystem.hpp"
#include "../Ecs/Systems/ScaleSystem.hpp"
#include "../Ecs/Systems/ButtonSystem.hpp"
#include "../Ecs/Systems/InputSystem.hpp"
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
    std::cout << "client constructeur" << std::endl;
    _send_structure.id = 2;
    send_to_server(_send_structure);
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
    _ecs.register_component<component::Button>();

    _background = _ecs.spawn_entity();
    _ecs.add_component(_background, component::Position(0.0f, 0.0f));
    _ecs.add_component(_background, component::Drawable("./assets/background.jpg"));
    if (!_music.openFromFile("./assets/game_music.ogg"))
        throw SFMLError("Music not found");

    _font = sf::Font();
    if (!_font.loadFromFile("src/Client/assets/font.ttf"))
        throw SFMLError("Font not found");

    // menu

    _btn_login = _ecs.spawn_entity();
    _btn_signin = _ecs.spawn_entity();
    _btn_back = _ecs.spawn_entity();

    _ecs.add_component(_btn_login, component::Position((sf::VideoMode::getDesktopMode().width / 2) + 120, sf::VideoMode::getDesktopMode().height / 2));
    _ecs.add_component(_btn_login, component::Scale(0.5f));
    _ecs.add_component(_btn_login, component::Button([this]() {
        this->updState(LOGIN);
        this->removeMenu();
        this->displayLogin();
    }, 379, 130, 0.5f));
    _ecs.add_component(_btn_login, component::AnimatedDrawable("assets/button_login.png", {2, 1}, {379, 130}, {1, 0}, {0, 0}, {0, 0}));

    _ecs.add_component(_btn_signin, component::Position((sf::VideoMode::getDesktopMode().width / 2) - 300, ( sf::VideoMode::getDesktopMode().height / 2)));
    _ecs.add_component(_btn_signin, component::Scale(0.5f));
    _ecs.add_component(_btn_signin, component::Button([this]() { 
        this->updState(SIGNIN);
        this->removeMenu();
        this->displaySignin();
    }, 395, 135, 0.5f));
    _ecs.add_component(_btn_signin, component::AnimatedDrawable("assets/button_signin.png", {2, 1}, {395, 135}, {1, 0}, {0, 0}, {0, 0}));

    
    _ecs.add_component(_btn_back, component::Position(20, 20));
    _ecs.add_component(_btn_back, component::Scale(0.05f));
    _ecs.add_component(_btn_back, component::Button([this]() {
        if (this->getStatus() == inGameState::LOGIN)
            this->removeLogin();
        else if (this->getStatus() == inGameState::SIGNIN)
            this->removeSignin();
        this->updState(MENU);
        this->removeLog();
        this->displayMenu();
    }, 512, 512, 0.5f));

    // login

    _inputLoginUsername = _ecs.spawn_entity();
    _inputLoginPassword = _ecs.spawn_entity();

    _usernameLogin = sf::Text("Username", _font, 30);
    _usernameLogin.setPosition(630, 265);
    _login_usernameSf = sf::Text("", _font, 25);
    _login_usernameSf.setPosition(630, 315);
    _login_usernameSf.setColor(sf::Color::Black);
    _ecs.add_component(_inputLoginUsername, component::Scale(0.5f));
    _ecs.add_component(_inputLoginUsername, component::Position(620.0f, 300.0f));
    _ecs.add_component(_inputLoginUsername, component::Input(&_login_usernameSf, _usernameStr, 528, 140));

    _passwordLogin = sf::Text("Password", _font, 30);
    _passwordLogin.setPosition(630, 465);
    _login_passwordSf = sf::Text("", _font, 25);
    _login_passwordSf.setPosition(630, 515);
    _login_passwordSf.setColor(sf::Color::Black);
    _ecs.add_component(_inputLoginPassword, component::Scale(0.5f));
    _ecs.add_component(_inputLoginPassword, component::Position(620.0f, 500.0f));
    _ecs.add_component(_inputLoginPassword, component::Input(&_login_passwordSf, _passwordStr, 528, 140));
    
    _buttonLogin = _ecs.spawn_entity();

    _ecs.add_component(_buttonLogin, component::Position(650, 600));
    _ecs.add_component(_buttonLogin, component::Scale(0.5f));
    _ecs.add_component(_buttonLogin, component::Button([this]() { 
        this->updState(CONNECTED);
        this->removeLog();
        this->removeLogin();
        this->displayConnected();
    }, 379, 130, 0.5f));

    // signin

    _buttonSignin = _ecs.spawn_entity();

    _ecs.add_component(_buttonSignin, component::Position(650, 600));
    _ecs.add_component(_buttonSignin, component::Scale(0.5f));
    _ecs.add_component(_buttonSignin, component::Button([this]() { 
        this->updState(CONNECTED);
        this->removeLog();
        this->removeSignin();
        this->displayConnected();
    }, 379, 130, 0.5f));

    // connected

    _btnPlay = _ecs.spawn_entity();

    _ecs.add_component(_btnPlay, component::Position(650, 550));
    _ecs.add_component(_btnPlay, component::Scale(0.5f));
    _ecs.add_component(_btnPlay, component::Button([this]() {
        this->updState(INGAME);
        this->removeConnected();
    }, 410, 104, 0.5f));


    InputSystem *tmp_input_sys = new InputSystem(_event, &_mouse_position, &_window);
    _ecs.add_system<component::Input, component::Position, component::Scale>(*tmp_input_sys);

    _window.create(sf::VideoMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height), "R-Type");
    _window.setFramerateLimit(60);
    listener.addRegistry(_ecs);
    SFMLDrawSystem *draw_sys = new SFMLDrawSystem(&_window, &_mouse_position);
    _ecs.add_system<component::Drawable, component::Position, component::Clickable, component::Hitbox>(*draw_sys);
    SFMLAnimatedDrawSystem *tmp_draw_sys = new SFMLAnimatedDrawSystem(&_window, &_mouse_position);
    _ecs.add_system<component::AnimatedDrawable, component::Position, component::Scale, component::Rotation>(*tmp_draw_sys);

    ButtonSystem *tmp_btn_sys = new ButtonSystem(&_mouse_position);
    _ecs.add_system<component::Button, component::Position>(*tmp_btn_sys);

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
    _state = MENU;
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

void Client::displayMenu()
{
    _ecs.add_component(_btn_login, component::AnimatedDrawable("assets/button_login.png", {2, 1}, {379, 130}, {1, 0}, {0, 0}, {0, 0}));
    _ecs.add_component(_btn_login, component::Button([this]() {
        this->updState(LOGIN);
        this->removeMenu();
        this->displayLogin();
    }, 379, 130, 0.5f));

    _ecs.add_component(_btn_signin, component::Button([this]() { 
        this->updState(SIGNIN);
        this->removeMenu();
        this->displaySignin();
    }, 395, 135, 0.5f));
    _ecs.add_component(_btn_signin, component::AnimatedDrawable("assets/button_signin.png", {2, 1}, {395, 135}, {1, 0}, {0, 0}, {0, 0}));
}

void Client::removeMenu()
{
    _ecs.remove_component<component::AnimatedDrawable>(_btn_signin);
    _ecs.remove_component<component::Button>(_btn_signin);
    _ecs.remove_component<component::AnimatedDrawable>(_btn_login);
    _ecs.remove_component<component::Button>(_btn_login);
}

void Client::drawLogin()
{
    std::cout << "dranw login" << std::endl;
    _window.draw(_usernameLogin);
    _window.draw(_passwordLogin);
    _window.draw(_login_usernameSf);
    _window.draw(_login_passwordSf);
}

void Client::displayLogin()
{
    _ecs.add_component(_inputLoginUsername, component::AnimatedDrawable("assets/input_img.png", {2, 1}, {528, 140}, {1, 0}, {0, 0}, {0, 0}));
    _ecs.add_component(_inputLoginUsername, component::Input(&_login_usernameSf, _usernameStr, 528, 140));
    _ecs.add_component(_inputLoginPassword, component::AnimatedDrawable("assets/input_img.png", {2, 1}, {528, 140}, {1, 0}, {0, 0}, {0, 0}));
    _ecs.add_component(_inputLoginPassword, component::Input(&_login_passwordSf, _passwordStr, 528, 140));
    _ecs.add_component(_buttonLogin, component::AnimatedDrawable("assets/button_login.png", {2, 1}, {395, 135}, {1, 0}, {0, 0}, {0, 0}));
    _ecs.add_component(_buttonLogin, component::Button([this]() { 
        this->updState(CONNECTED);
        this->removeLog();
        this->removeLogin();
        this->displayConnected();
    }, 379, 130, 0.5f));
    _ecs.add_component(_btn_back, component::AnimatedDrawable("assets/back.png", {2, 1}, {512, 512}, {1, 0}, {0, 0}, {0, 0}));
    _ecs.add_component(_btn_back, component::Button([this]() {
        if (this->getStatus() == inGameState::LOGIN)
            this->removeLogin();
        else if (this->getStatus() == inGameState::SIGNIN)
            this->removeSignin();
        this->updState(MENU);
        this->removeLog();
        this->displayMenu();
    }, 512, 512, 0.5f));
}

void Client::drawSignin()
{
    std::cout << "dranw signin" << std::endl;
    _window.draw(_usernameLogin);
    _window.draw(_passwordLogin);
    _window.draw(_login_usernameSf);
    _window.draw(_login_passwordSf);
}

void Client::displaySignin()
{
    _ecs.add_component(_inputLoginUsername, component::AnimatedDrawable("assets/input_img.png", {2, 1}, {528, 140}, {1, 0}, {0, 0}, {0, 0}));
    _ecs.add_component(_inputLoginUsername, component::Input(&_login_usernameSf, _usernameStr, 528, 140));
    _ecs.add_component(_inputLoginPassword, component::AnimatedDrawable("assets/input_img.png", {2, 1}, {528, 140}, {1, 0}, {0, 0}, {0, 0}));
    _ecs.add_component(_inputLoginPassword, component::Input(&_login_passwordSf, _passwordStr, 528, 140));
    _ecs.add_component(_buttonSignin, component::AnimatedDrawable("assets/button_signin.png", {2, 1}, {395, 135}, {1, 0}, {0, 0}, {0, 0}));
    _ecs.add_component(_buttonSignin, component::Button([this]() { 
        this->updState(CONNECTED);
        this->removeLog();
        this->removeSignin();
        this->displayConnected();
    }, 379, 130, 0.5f));
    _ecs.add_component(_btn_back, component::AnimatedDrawable("assets/back.png", {2, 1}, {512, 512}, {1, 0}, {0, 0}, {0, 0}));
    _ecs.add_component(_btn_back, component::Button([this]() {
        if (this->getStatus() == inGameState::LOGIN)
            this->removeLogin();
        else if (this->getStatus() == inGameState::SIGNIN)
            this->removeSignin();
        this->updState(MENU);
        this->removeLog();
        this->displayMenu();
    }, 512, 512, 0.5f));
}

void Client::removeLog()
{
    _ecs.remove_component<component::AnimatedDrawable>(_inputLoginUsername);
    _ecs.remove_component<component::AnimatedDrawable>(_inputLoginPassword);
    _ecs.remove_component<component::Input>(_inputLoginUsername);
    _ecs.remove_component<component::Input>(_inputLoginPassword);
    _ecs.remove_component<component::AnimatedDrawable>(_btn_back);
    _ecs.remove_component<component::Button>(_btn_back);
}

void Client::displayConnected()
{
    _ecs.add_component(_btnPlay, component::AnimatedDrawable("assets/button_playing.png", {2, 1}, {410, 104}, {1, 0}, {0, 0}, {0, 0}));
}

void Client::removeLogin()
{
    std::cout << "remove Login" << std::endl;
    _ecs.remove_component<component::AnimatedDrawable>(_buttonLogin);
    _ecs.remove_component<component::Button>(_buttonLogin);
}

void Client::removeSignin()
{
    std::cout << "remove signin" << std::endl;
    _ecs.remove_component<component::AnimatedDrawable>(_buttonSignin);
    _ecs.remove_component<component::Button>(_buttonSignin);
}

void Client::removeConnected()
{
    _ecs.remove_component<component::AnimatedDrawable>(_btnPlay);
    _ecs.remove_component<component::Button>(_btnPlay);
}

void Client::updState(inGameState state)
{
    this->_state = state;
}

inGameState Client::getStatus()
{
    return _state;
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
        if (_state == MENU) {
            _ecs.run_systems();
        }
        if (_state == SIGNIN) {
            _ecs.run_systems();
            drawSignin();
        }
        if (_state == LOGIN) {
            _ecs.run_systems();
            drawLogin();
        }
        if (_state == CONNECTED) {
            _ecs.run_systems();
        }
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
