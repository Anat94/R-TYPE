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
#include "../Ecs/Systems/ParallaxSystem.hpp"
#include "../Ecs/Systems/RotationSystem.hpp"
#include "../Ecs/Systems/ControlSystem.hpp"
#include "../Ecs/Systems/ScaleSystem.hpp"
#include "../Ecs/Systems/ButtonSystem.hpp"
#include "../KeyEventMapping.hpp"

bool can_read = true;

/**
 * @brief Receive data from the server and store them
 *
 * @return std::vector<char>
 */

std::vector<char> Client::receive_raw_data_from_client()
{
    std::vector<char> receivedData(MAX_BUF_SIZE);

    mtx.unlock();
    size_t bytesRead = _socket.receive_from(asio::buffer(receivedData), _server_endpoint);
    mtx.lock();
    receivedData.resize(bytesRead);

    return receivedData;
}

/**
 * @brief get best 3 highscores from server
 *
 * @param server_msg The message from the server
 * @return The packet id
 */

int Client::receive_high_score(std::vector<char> &server_msg)
{
    _logger.log(CLIENT, "Received high score");
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

/**
 * @brief receive from the server a room join event
 * 
 * @param server_msg raw server message
 * @return int 
 */
int Client::receive_room_join_event(std::vector<char> &server_msg)
{
    _logger.log(CLIENT, "Received room join event");
    if (server_msg.size() < sizeof(RoomJoinMessage))
        return -1;
    RoomJoinMessage *snapshot = reinterpret_cast<RoomJoinMessage *>(server_msg.data());
    if (snapshot->room_name[0] == '\0') {
        return snapshot->id;
    }
    std::cout << "Successfully joined room: " << snapshot->room_name << "!\n";
    _room_name = snapshot->room_name;
    return snapshot->id;
}

/**
 * @brief receive from the server a room creation event
 *
 * @param server_msg raw server message
 * @return int
 */
int Client::recieve_room_creation_event(std::vector<char> &server_msg)
{
    _logger.log(CLIENT, "Received room creation event");
    if (server_msg.size() < sizeof(RoomCreationMessage))
        return -1;
    RoomCreationMessage *snapshot = reinterpret_cast<RoomCreationMessage *>(server_msg.data());
    if (snapshot->room_name[0] == '\0') {
        std::cout << "unable to create room" << std::endl;
        return snapshot->id;
    }
    std::cout << "Successfully created room: " << snapshot->room_name << "!\n";
    _room_name = snapshot->room_name;
    return snapshot->id;
}

/**
 * @brief get when a player dead
 *
 * @param server_msg The message from the server
 * @return The packet id
 */

int Client::receive_death_event(std::vector<char> &server_msg)
{
    _logger.log(CLIENT, "Received death event");
    if (server_msg.size() < sizeof(DeathEventMessage))
        return -1;
    DeathEventMessage *snapshot = reinterpret_cast<DeathEventMessage *>(server_msg.data());
    while (!can_read)
        continue;
    try {
        entity_t real_entity = get_entity_from_server_entity(snapshot->entity);
        if (real_entity > 0)
            _listener.addEvent(new DeathEvent(real_entity, -1));
    } catch (const std::exception &ex) {
        std::cout << ex.what() << std::endl;
    }
    return snapshot->packet_id;
}

/**
 * @brief Get client entity from the server's entity
 * 
 * @param srvEntity server entity
 * @return client entity
 */
entity_t Client::get_entity_from_server_entity(entity_t srvEntity)
{
    auto &srv = _ecs.get_components<component::ServerEntity>();

    for (size_t i = 0; i < srv.size(); ++i) {
        if (srv[i].has_value()) {
            if (srv[i]->entity == srvEntity)
                return i;
        }
    }
    return 0;
}

/**
 * @brief get the player position from the server
 *
 * @param server_msg The message from the server
 * @return int The packet id
 */

int Client::receive_position_snapshot_update(std::vector<char> &server_msg)
{
    if (server_msg.size() < sizeof(SnapshotPosition))
        return -1;
    SnapshotPosition *snapshot = reinterpret_cast<SnapshotPosition *>(server_msg.data());
    sparse_array<component::Position> &pos = _ecs.get_components<component::Position>();
    while (!can_read)
        continue;
    try {
        entity_t real_entity = get_entity_from_server_entity(snapshot->entity);
        if (real_entity > 0 && pos[real_entity].has_value()) {
            if (std::abs(pos[real_entity]->x - snapshot->data.x) < MAX_POSITION_MOVE_THRESHOLD &&
                std::abs(pos[real_entity]->y - snapshot->data.y) < MAX_POSITION_MOVE_THRESHOLD) {
                pos[real_entity]->x = snapshot->data.x;
                pos[real_entity]->y = snapshot->data.y;
            }
        } else {
            if (real_entity == 0)
                real_entity = init_new_entity(snapshot->entity);
            _ecs.add_component(real_entity, component::Position(snapshot->data.x,  snapshot->data.y));
        }
    } catch (const std::exception &ex) {
        std::cout << ex.what() << std::endl;
    }
    return snapshot->packet_id;
}

/**
 * @brief initialize a new entity
 * 
 * @param srvEntity server entity to initialize for
 * @return created entity 
 */
entity_t Client::init_new_entity(entity_t srvEntity)
{
    entity_t new_entity = _ecs.spawn_entity();
    _ecs.add_component(new_entity, component::ServerEntity(srvEntity));
    _ecs.add_component(new_entity, component::Health(100));
    _ecs.add_component(new_entity, component::Score());
    return new_entity;
}

/**
 * @brief get the scale from the server
 *
 * @param server_msg The message from the server
 * @return int  The packet id
 */
int Client::receive_scale_snapshot_update(std::vector<char> &server_msg)
{
    if (server_msg.size() < sizeof(ScaleSnapshot))
        return -1;
    ScaleSnapshot *snapshot = reinterpret_cast<ScaleSnapshot *>(server_msg.data());
    sparse_array<component::Scale> &scale = _ecs.get_components<component::Scale>();
    while (!can_read)
        continue;
    try {
        entity_t real_entity = get_entity_from_server_entity(snapshot->entity);
        if (real_entity > 0 && scale[real_entity].has_value()) {
            scale[real_entity]->_scale.first = snapshot->data._scale.first;
            scale[real_entity]->_scale.second = snapshot->data._scale.second;
        } else {
            if (real_entity == 0)
                real_entity = init_new_entity(snapshot->entity);
            _ecs.add_component(real_entity, component::Scale(snapshot->data));
        }
    } catch (const std::exception &ex) {
        std::cout << ex.what() << std::endl;
    }
    return snapshot->packet_id;
}

/**
 * @brief while login, this get the login response from server
 *
 * @param server_msg The message from the server
 * @return int  The packet id
 */
int Client::receive_login_response(std::vector<char> &server_msg)
{

    if (server_msg.size() < sizeof(LoginResponse))
        return -1;
    LoginResponse *login = reinterpret_cast<LoginResponse *>(server_msg.data());
    if (login->response == true && login->logintype == 1) {
        std::cout << "Connected" << std::endl;
        _logged_in = true;
        _logger.log(CLIENT, "Client is connected successfully");
        return login->packet_id;
    } else if (login->response == false  && login->logintype == 1) {
        std::cout << "Wrong credentials" << std::endl;
        _logger.log(CLIENT, "Client is not connected");
        return login->packet_id;
    } else if (login->response == true && login->logintype == 0) {
        std::cout << "Registered" << std::endl;
        _logger.log(CLIENT, "Client is registered successfully");
        _logged_in = true;
        return login->packet_id;
    } else if (login->response == false && login->logintype == 0) {
        std::cout << "An error occured whil registring" << std::endl;
        _logger.log(CLIENT, "Client is not registered");
        return login->packet_id;
    }
    return -1;
}

/**
 * @brief get friends from server
 *
 * @param server_msg The message from the server
 * @return int  The packet id
 */
int Client::receive_friends_reponse(std::vector<char> &server_msg) {
    _logger.log(CLIENT, "Received friends response");
    if (server_msg.size() < sizeof(FriendsResponse))
        return -1;
    FriendsResponse *friends = reinterpret_cast<FriendsResponse *>(server_msg.data());
    friendLists = friends->friends;
    return friends->packet_id;
}

/**
 * @brief response of adding friends from the server
 *
 * @param server_msg The message from the server
 * @return int  The packet id
 */
int Client::receive_add_friends_reponse(std::vector<char> &server_msg) {
    _logger.log(CLIENT, "Received add friends response");
    if (server_msg.size() < sizeof(AddFriendsResponse))
        return -1;
    AddFriendsResponse *friends = reinterpret_cast<AddFriendsResponse *>(server_msg.data());
    if (friends->response == true)
        std::cout << "Friend added" << std::endl;
    else
        std::cout << "An error occured while adding friend" << std::endl;
    return friends->packet_id;
}

/**
 * @brief reponse of removing friends from the server
 *
 * @param server_msg The message from the server
 * @return int  The packet id
 */
int Client::receive_remove_friends_reponse(std::vector<char> &server_msg) {
    _logger.log(CLIENT, "Received remove friends response");
    if (server_msg.size() < sizeof(RemoveFriendsResponse))
        return -1;
    RemoveFriendsResponse *friends = reinterpret_cast<RemoveFriendsResponse *>(server_msg.data());
    if (friends->response == true)
        std::cout << "Friend removed" << std::endl;
    else
        std::cout << "An error occured while removing friend" << std::endl;
    return friends->packet_id;
}

/**
 * @brief receive a chat from the server
 *
 * @param server_msg The message from the server
 * @return int  The packet id
 */
int Client::receive_chat_event(std::vector<char> &server_msg)
{
    _logger.log(CLIENT, "Received chat event");
    if (server_msg.size() < sizeof(ChatMessage))
        return -1;
    ChatMessage *chat = reinterpret_cast<ChatMessage *>(server_msg.data());
    std::cout << chat->name <<": " << chat->content << std::endl;
    _chatEntity._chat.push_back(std::string(chat->name) + ": " + std::string(chat->content));
    sf::Text tmp;
    tmp.setString(std::string(chat->name) + ":    " + std::string(chat->content));
    tmp.setFont(_font);
    tmp.setCharacterSize(20);
    tmp.setPosition(30, 150 + (_chatEntity._chatText.size() - 1) * 30);
    _chatEntity._chatText.push_back(tmp);
    return chat->packet_id;
}

/**
 * @brief receive thr drawable from the server
 *
 * @param server_msg The message from the server
 * @return int  The packet id
 */
int Client::receive_drawable_snapshot_update(std::vector<char> &server_msg)
{
    if (server_msg.size() < sizeof(DrawableSnapshot))
        return -1;

    DrawableSnapshot *snapshot = reinterpret_cast<DrawableSnapshot *>(server_msg.data());
    sparse_array<component::Drawable> &drawables = _ecs.get_components<component::Drawable>();
    while (!can_read)
        continue;
    try {
        entity_t real_entity = get_entity_from_server_entity(snapshot->entity);
        if (real_entity > 0 && drawables[real_entity].has_value()) {
            drawables[real_entity]->_path = std::string(snapshot->data);
        } else {
            if (real_entity == 0)
                real_entity = init_new_entity(snapshot->entity);
            _ecs.add_component(real_entity, component::Drawable(std::string(snapshot->data)));
        }
    } catch (const std::exception &ex) {
        std::cout << ex.what() << std::endl;
    }
    return snapshot->packet_id;
}

/**
 * @brief receive the animated drawable from the server
 * 
 * @param server_msg The message from the server
 * @return int  The packet id
 */
int Client::receive_animated_drawable_snapshot(std::vector<char> &server_msg)
{
    if (server_msg.size() < sizeof(AnimatedDrawableSnapshot))
        return -1;
    AnimatedDrawableSnapshot *snapshot = reinterpret_cast<AnimatedDrawableSnapshot *>(server_msg.data());
    sparse_array<component::AnimatedDrawable> &drawables = _ecs.get_components<component::AnimatedDrawable>();
    while (!can_read)
        continue;
    try {
        entity_t real_entity = get_entity_from_server_entity(snapshot->entity);
        if (real_entity > 0 && drawables[real_entity].has_value()) {
            drawables[real_entity]->_state = std::string(snapshot->_state);
        } else {
            if (real_entity == 0)
                real_entity = init_new_entity(snapshot->entity);
            _ecs.add_component(real_entity, component::AnimatedDrawable(snapshot->_path, snapshot->_nbSprites, snapshot->_spriteSize, snapshot->_gaps, snapshot->_firstOffset, snapshot->_currentIdx));
            auto &tmp = _ecs.get_components<component::AnimatedDrawable>()[real_entity];
            for (size_t i = 0; i < snapshot->_anims.size(); i++) {
                if (snapshot->_anims[i].first[0] == '\0')
                    continue;
                tmp->addAnimation(std::string(snapshot->_anims[i].first), snapshot->_anims[i].second.second, snapshot->_anims[i].second.first);
            }
        }
    } catch (const std::exception &ex) {
        std::cout << ex.what() << std::endl;
    }
    return snapshot->packet_id;
}

/**
 * @brief receive animation update from the server
 *
 * @param server_msg The message from the server
 * @return int  The packet id
 */
int Client::receive_animated_drawable_state_update(std::vector<char> &server_msg)
{
    if (server_msg.size() < sizeof(AnimatedStateUpdateMessage))
        return -1;
    AnimatedStateUpdateMessage *snapshot = reinterpret_cast<AnimatedStateUpdateMessage *>(server_msg.data());
    sparse_array<component::AnimatedDrawable> &drawables = _ecs.get_components<component::AnimatedDrawable>();
    while (!can_read)
        continue;
    try {
        entity_t real_entity = get_entity_from_server_entity(snapshot->entity);
        if (real_entity > 0 && drawables[real_entity].has_value()) {
            drawables[real_entity]->_state = std::string(snapshot->state);
        }
    } catch (const std::exception &ex) {
        std::cout << ex.what() << std::endl;
    }
    return snapshot->packet_id;
}

/**
 * @brief receive health event from the server
 *
 * @param server_msg raw server message
 * @return packet id
 */
int Client::receive_health_event(std::vector<char> &server_msg)
{
    if (server_msg.size() < sizeof(HealthMessage))
        return 84;
    HealthMessage *snapshot = reinterpret_cast<HealthMessage *>(server_msg.data());
    try {
        _lives = snapshot->health;
        _lives_text.setString("Health: " + std::to_string(_lives));
    } catch (const std::exception &ex) {
        std::cout << ex.what() << std::endl;
    }
    return snapshot->packet_id;
}

/**
 * @brief receive score event from the server
 * 
 * @param server_msg raw server message
 * @return packet id
 */
int Client::receive_score_event(std::vector<char> &server_msg)
{
    if (server_msg.size() < sizeof(ScoreMessage))
        return 84;
    ScoreMessage *snapshot = reinterpret_cast<ScoreMessage *>(server_msg.data());
    try {
        _score = snapshot->score;
    } catch (const std::exception &ex) {
        std::cout << ex.what() << std::endl;
    }
    return snapshot->packet_id;
}

/**
 * @brief Receive event from the server
 *
 * @param server_msg The message from the server
 * @return int  The packet id
 */
void Client::receive()
{
    mtx.lock();
    if (prgrmstop)
        exit(0);
    std::vector<char> server_msg = receive_raw_data_from_client();
    if (server_msg.size() < sizeof(BaseMessage))
        return;
    BaseMessage *baseMsg = reinterpret_cast<BaseMessage *>(server_msg.data());
    
    int check_if_packet_exist = 0;
    for (const auto &element: _packets_received) {
        if (element == baseMsg->packet_id)
            check_if_packet_exist = 1;
    }
    if (_messageParser.find(baseMsg->id) == _messageParser.end())
        throw ArgumentError("ERROR: Invalid event received: " + std::to_string(baseMsg->id) + ".");
    if (check_if_packet_exist == 0) {
        (this->*_messageParser[baseMsg->id])(server_msg);
        _packets_received.push_back(baseMsg->packet_id);
    }
    ConfirmationMessage to_send;
    to_send.id = 5;
    to_send.packet_id = baseMsg->packet_id;
    send_to_server<ConfirmationMessage>(to_send);
    mtx.unlock();
    receive();
}

/**
 * @brief Construct a new Client:: Client object
 *
 * @param ip ip to connect
 * @param port port to connect
 * @param listener event listener
 * @param ecs ecs registry
 * @param mtx_ mutex for thread safety
 */
Client::Client(std::string ip, int port, EventListener &listener, registry &ecs, std::mutex &mtx_)
    : _io_context(),
      _socket(_io_context, udp::endpoint(udp::v4(), 0)),
      _server_endpoint(udp::endpoint(asio::ip::make_address(ip), port)),
      _listener(listener),
      _ecs(ecs),
      mtx(mtx_),
      _logger(CLIENT)
{
    _logger.log(CLIENT, "Client is starting");
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
    _chatEntity._rectangle = sf::RectangleShape(sf::Vector2f(400, 1100));
    _chatEntity._rectangle.setPosition(0.0, 0.0);
    _chatEntity._rectangle.setFillColor(sf::Color::Black);
    _chatEntity._chatTitle  = sf::Text("Chat", _font, 30);
    _chatEntity._chatTitle.setPosition(150, 50);
    _chatEntity._inputBox = sf::RectangleShape(sf::Vector2f(350, 50));
    _chatEntity._inputBox.setPosition(25.0, 920.0);
    _chatEntity._inputBox.setFillColor(sf::Color::White);
    _chatEntity._input = std::string("");
    _chatEntity._chatTextInput.setString(_chatEntity._input);
    _chatEntity._chatTextInput.setFont(_font);
    _chatEntity._chatTextInput.setCharacterSize(20);
    _chatEntity._chatTextInput.setPosition(50, 920);
    _chatEntity._chatTextInput.setFillColor(sf::Color::Black);
    receiveThread = std::thread(&Client::receive, this);
}


/**
 * @brief Destroy the Client:: Client object
 *
 */
Client::~Client()
{
    _logger.log(CLIENT, "Client is stopping");
    _font.~Font();
    _music.stop();
    prgrmstop = true;
    if (receiveThread.joinable())
        receiveThread.join();
}

/**
 * @brief send a structure to the server
 *
 * @tparam T
 * @param structure stucture to send
 */
template <typename T>
void Client::send_to_server(const T& structure) {
    _socket.send_to(asio::buffer(&structure, sizeof(structure)), _server_endpoint);
}

/**
 * @brief display on-screen texts.
 *
 */
void Client::displayTexts()
{
    _window.draw(_score_text);
    _window.draw(_lives_text);
    _window.draw(_level_text);
    _window.draw(_mouse_position_text);
}

/**
 * @brief manage event of the window
 *
 * @return int 0 if no error, 1 on error or on window close
 */
int Client::manageEvent()
{
    while (_window.pollEvent(_event)) {
        handleInput(_event);
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
            if (_event.key.code == sf::Keyboard::Tab) {
                _state = (_state == INGAME) ?  CHAT : (INGAME);
                return 0;
            }
            if (_event.key.code == sf::Keyboard::Enter) {
                ChatMessage msg(12, _username, _chatEntity._input, _packet_id);
                _packet_id++;
                send_to_server(msg);
                _chatEntity._input = "";
                return 0;
            }
        }
        if (std::find(eventsToPrint.begin(), eventsToPrint.end(), _event.type) != eventsToPrint.end()) {
            _send_structure.id = 1;
            if (SFMLKeys.find(_event.key.code) != SFMLKeys.end())
                _send_structure.event = KeyIds[SFMLKeys[_event.key.code]];
            else
                _send_structure.event = -1;
            if (SFMLKeys[_event.key.code] == "Space") {
                if (shootTimer.getElapsedTime() > 500) {
                    send_to_server(_send_structure);
                    shootTimer.restart();
                }
            } else {
                if (moveTimer.getElapsedTime() > 10) {
                    send_to_server(_send_structure);
                    moveTimer.restart();
                }
            }
            _event = _event;
            return 0;
        }
    }
    return 0;
}

/**
 * @brief Display the scoreboard menu
 *
 */
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

/**
 * @brief Handle input from the user
 *
 * @param event Event of the window
 */
void Client::handleInput(sf::Event& event) {
    if (event.type == sf::Event::TextEntered ) {
        if (event.text.unicode < 128) {
            if (event.text.unicode == '\b' && _chatEntity._input.size() > 0) {
                _chatEntity._input.erase(_chatEntity._input.size() - 1, 1);
            } else if (event.text.unicode != '\b') {
                _chatEntity._input += event.text.unicode;
            }
            _chatEntity._chatTextInput.setString(_chatEntity._input);
        }
    }
}

/**
 * @brief manage the client cli at the beginning of the program
 *
 */
void Client::manageCli()
{
    std::string input;
    std::string command;
    std::string params;
    std::string param2;
    while (true) {
        std::cout << ">> ";
        std::getline(std::cin, input);
        std::istringstream iss(input);
        std::getline(iss, command, ' ');
        std::getline(iss, params, ' ');
        std::getline(iss, param2, ' ');
        if (command == "EXIT")
            exit(0);
        else if (command == "SIGNIN") {
            if (params == "" || param2 == "") {
                std::cout << "Usage: SIGNIN [username] [password]" << std::endl;
                continue;
            } else {
                LoginMessage login(17, params, param2, 1, _packet_id); // 0 == signup & 1 == signin
                _packet_id += 1;
                send_to_server<LoginMessage>(login);
                _username = params;
            }
        } else if (command == "SIGNUP") {
            std::cout << "SIGNUP" << std::endl;
            std::cout << params << std::endl;
            std::cout << param2 << std::endl;
            if (params == "" || param2 == "") {
                std::cout << "Usage: SIGNUP [username] [password]" << std::endl;
                continue;
            } else {
                LoginMessage login(17, params, param2, 0, _packet_id); // 0 == signup & 1 == signin
                _packet_id += 1;
                send_to_server<LoginMessage>(login);
                _username = params;
            }
        } else if (command == "START") {
            _logger.log(CLIENT, "Received start command");
            if (_username == "") {
                std::cout << "You must be connected to run this command - see HELP" << std::endl;
                continue;
            }
            if (_room_name == "") {
                std::cout << "You must be in a room to start the game - see HELP" << std::endl;
                continue;
            }
            JoinGameMessage to_send(2, _username, _room_name, spectator_mode, _room_mode, _packet_id);
            _packet_id++;
            send_to_server(to_send);
            return;
        } else if (command == "CREATE") {
            _logger.log(CLIENT, "Received create command");
            if (_username == "") {
                std::cout << "You must be connected to run this command - see HELP" << std::endl;
                continue;
            }
            if (params == "" ) {
                std::cout << "Usage: CREATE [room_name]" << std::endl;
                continue;
            }
            RoomCreationMessage to_send(21, _username, params, _packet_id);
            _packet_id++;
            send_to_server<RoomCreationMessage>(to_send);
        } else if (command == "JOIN") {
            _logger.log(CLIENT, "Received join command");
            if (_username == "") {
                std::cout << "You must be connected to run this command - see HELP" << std::endl;
                continue;
            }
            if (params == "" ) {
                std::cout << "Usage: JOIN [room_name]" << std::endl;
                continue;
            }
            RoomJoinMessage to_send(22, params, _packet_id);
            _packet_id++;
            send_to_server<RoomJoinMessage>(to_send);
        } else if (command == "SPECMODE") {
            if (params.size() < 1) {
                std::cout << "Usage: SPECMODE [1 || 0]" << std::endl;
                continue;
            }
            try {
                spectator_mode = std::stoi(params) == 1 ? true : false;
            } catch( std::exception &e) {
                std::cout << "Usage: SPECMODE [1 || 0]" << std::endl;
            }
        } else if (command == "LIST_FRIENDS") {
            _logger.log(CLIENT, "Received list friends command");
            if (_username == "") {
                std::cout << "You must be connected to run this command - see HELP" << std::endl;
                continue;
            }
            if (friendLists.size() > 0)
                friendLists.clear();
            FriendsMessage friendsmsg(18, _username, _packet_id);
            _packet_id += 1;
            send_to_server<FriendsMessage>(friendsmsg);
            std::cout << "Friends list:" << std::endl;
            while (friendLists.size() == 0) {
                continue;
            }
            std::cout << friendLists;
        } else if (command == "ADD_FRIENDS") {
            _logger.log(CLIENT, "Received add friends command");
            if (_username == "") {
                std::cout << "You must be connected to run this command - see HELP" << std::endl;
                continue;
            }
            if (params != "") {
                AddFriendsMessage add(19, _username, params,  _packet_id);
                _packet_id += 1;
                send_to_server<AddFriendsMessage>(add);
            } else {
                std::cout << "Usage: ADD_FRIENDS [name]" << std::endl;
            }
        } else if (command == "REMOVE_FRIENDS") {
            _logger.log(CLIENT, "Received remove friends command");
            if (_username == "") {
                std::cout << "You must be connected to run this command - see HELP" << std::endl;
                continue;
            }
            if (params != "") {
                printf("%s\n", params.c_str());
                RemoveFriendsMessage remove(20, _username, params,  _packet_id);
                _packet_id += 1;
                send_to_server<RemoveFriendsMessage>(remove);
            } else {
                std::cout << "Usage: REMOVE_FRIENDS [name]" << std::endl;
            }
        } else if (command == "CHAT") {
            _logger.log(CLIENT, "Received chat command");
            if (_username == "") {
                std::cout << "You must be connected to run this command - see HELP" << std::endl;
                continue;
            }
            if (params != "") {
                ChatMessage msg(12, _username, params, _packet_id);
                _packet_id +=1;
                send_to_server<ChatMessage>(msg);
            } else {
                std::cout << "Usage: CHAT [message]" << std::endl;
            }
        } else if (command == "WHOAMI") {
            std::cout << "You are " << _username << std::endl;
        } else if (command == "HELP") {
            if (params == "SET") {
                std::cout << "SET [rule] [value]: modifies a rule to the value" << std::endl;
                std::cout << "Rules available:" << std::endl;
                std::cout << " - mode 0|1: sets the mode to infinite (0) or campaign (1)" << std::endl;
            } else {
                std::cout << "Available commands:" << std::endl;
                std::cout << "START: Start the game" << std::endl;
                std::cout << "CREATE [room_name]: Create a room" << std::endl;
                std::cout << "JOIN [room_name]: Join a room" << std::endl;
                std::cout << "SIGNIN [name] [password]: Signin" << std::endl;
                std::cout << "SIGNUP [name] [password]: Signup" << std::endl;
                std::cout << "WHOAMI: Show who you are" << std::endl;
                std::cout << "SET [rule] [value]: modifies a rule to the value" << std::endl;
                std::cout << "LIST_FRIENDS: List all your friends" << std::endl;
                std::cout << "ADD_FRIENDS [name]: Add a friend" << std::endl;
                std::cout << "REMOVE_FRIENDS [name]: Remove a friend" << std::endl;
                std::cout << "CHAT [message]: Send a message to all your friends" << std::endl;
                std::cout << "HIGHSCORE: Display the highscore" << std::endl;
                std::cout << "CLEAR: Clear the terminal" << std::endl;
                std::cout << "EXIT: Exit the game" << std::endl;
            }
        } else if (command == "CLEAR") {
            system("clear");
        } else if(command == "SET") {
            if (params == "" || param2 == "") {
                std::cout << "Usage: SET [rule] [value]" << std::endl;
                continue;
            } else {
                if (_username == "") {
                    std::cout << "You must be connected to run this command - see HELP" << std::endl;
                    continue;
                }
                if (params != "mode") {
                    std::cout << "Invalid rule - see HELP SET" << std::endl;
                    continue;
                }
                if (std::atoi(param2.c_str()) < 0 || 1 < std::atoi(param2.c_str())) {
                    std::cout << "Invalid value for SET - see HELP SET" << std::endl;
                    continue;
                } else {
                    if (std::atoi(param2.c_str())) {
                        _room_mode = 1;
                        std::cout << "Set mode to campaign" << std::endl;
                    } else {
                        _room_mode = 0;
                        std::cout << "Set mode to infinite" << std::endl;
                    }
                }
            }
        } else {
            std::cout << "Command not found" << std::endl;
        }
    }
}


/**
 * @brief Init client class
 *
 */
void Client::initClass()
{
    _window.create(sf::VideoMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height), "R-Type");
    _logger.log(CLIENT, "Window created");
    _window.setFramerateLimit(60);
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
    _ecs.register_component<component::Parallax>();
    _ecs.register_component<component::Position>();
    _ecs.register_component<component::Rotation>();
    _ecs.register_component<component::Velocity>();
    _ecs.register_component<component::Clickable>();
    _ecs.register_component<component::Controllable>();
    _ecs.register_component<component::ResetOnMove>();
    _ecs.register_component<component::ServerEntity>();
    _ecs.register_component<component::AnimatedDrawable>();
    _ecs.register_component<component::HurtsOnCollision>();
    _ecs.register_component<component::Shield>();
    _ecs.register_component<component::ShootCounter>();
    _ecs.register_component<component::KillOnTimer>();
    _ecs.register_component<component::AutoShoot>();
    _background = _ecs.spawn_entity();
    entity_t _background2 = _ecs.spawn_entity();
    entity_t _background3 = _ecs.spawn_entity();
    _ecs.add_component(_background, component::Position(0.0f, 0.0f));
    _ecs.add_component(_background, component::Drawable("assets/parallax-space-background.png"));
    _ecs.add_component(_background, component::Parallax(1, 0));
    _ecs.add_component(_background2, component::Position(0.0f, 0.0f));
    _ecs.add_component(_background2, component::Drawable("assets/parallax-space-stars.png"));
    _ecs.add_component(_background2, component::Parallax(1, 1));
    _ecs.add_component(_background3, component::Position(0.0f, 0.0f));
    _ecs.add_component(_background3, component::Drawable("assets/parallax-space-far-planets.png"));
    _ecs.add_component(_background3, component::Parallax(1, 2));
    if (!_music.openFromFile("./assets/game_music.ogg"))
        throw SFMLError("Music not found");
    _listener.addRegistry(_ecs);
    SFMLDrawSystem *draw_sys = new SFMLDrawSystem(&_window, &_mouse_position);
    _ecs.add_system<component::Drawable, component::Position, component::Hitbox, component::Parallax>(*draw_sys);
    SFMLAnimatedDrawSystem *tmp_draw_sys = new SFMLAnimatedDrawSystem(&_window, &_mouse_position);
    _ecs.add_system<component::AnimatedDrawable, component::Position, component::Scale, component::Rotation>(*tmp_draw_sys);
    ParallaxSystem *par_sys = new ParallaxSystem();
    _ecs.add_system<component::Parallax, component::Position>(*par_sys);
}

/**
 * @brief run the client window
 *
 * @return int return 0
 */
int Client::run()
{
    manageCli();
    mtx.lock();
    initClass();
    _music.play();
    _music.setLoop(true);
    _music.setVolume(25);
    _lives = 0; // ((player1_h.has_value()) ? (player1_h->_health) : (0));
    _score = 0; // ((player1_s.has_value()) ? (player1_s->_score) : (0));
    _lives_text.setPosition(1750, 10);
    mtx.unlock();
    while (_window.isOpen()) {
        _mouse_position = sf::Mouse::getPosition(_window);
        mtx.lock();
        _score_text.setString("Score: " + std::to_string(_score));
        _lives_text.setString("Health: " + std::to_string(_lives));
        if (manageEvent()) {
            _window.close();
            break;
        }
        if (_state == INGAMEMENU)
            displayScoreBoardMenu();
        else if (_state == INGAME || _state == CHAT) {
            _mouse_position_text.setString("Mouse: " + std::to_string(_mouse_position.x) + ", " + std::to_string(_mouse_position.y));
            while (_listener.popEvent());
            _ecs.run_systems();
            displayTexts();
            if (_state == CHAT) {
                _window.draw(_chatEntity._rectangle);
                _window.draw(_chatEntity._chatTitle);
                _window.draw(_chatEntity._inputBox);
                _window.draw(_chatEntity._chatTextInput);
                for (size_t i = 0; i < _chatEntity._chat.size(); i++) {
                    _window.draw(_chatEntity._chatText[i]);
                }
                if (_chatEntity._chat.size() > 25) {
                    _chatEntity._chat.erase(_chatEntity._chat.begin());
                    _chatEntity._chatText.erase(_chatEntity._chatText.begin());
                    for (size_t i = 0; i < _chatEntity._chatText.size(); i++) {
                        _chatEntity._chatText[i].setPosition(30, 150 + i * 30);
                    }
                }
            }
        }
        mtx.unlock();
        _window.display();
    }
    return 0;
}
