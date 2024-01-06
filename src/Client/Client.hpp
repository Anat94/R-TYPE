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
    int event;
};

struct HighScoreMessage: public BaseMessage {
    int score1;
    char name1[20];
    int score2;
    char name2[20];
    int score3;
    char name3[20];
    HighScoreMessage(int16_t id_, std::string name1_, int score1_, int packet_id_):
        score1(score1_) {
            int i = 0;
            id = id_;
            packet_id = packet_id_;
            for (; i < name1_.size(); i++) {
                name1[i] = name1_[i];
            }
            name1[i] = '\0';
        };
};

struct LoginMessage: public BaseMessage {
    char username[20];
    char password[20];
    int logintype;
    LoginMessage(int16_t id_, std::string username_, std::string password_, int type_, int packet_id_):
        username(), password(), logintype(type_) {
            int i = 0;
            id = id_;
            packet_id = packet_id_;
            for (; i < username_.size(); i++) {
                username[i] = username_[i];
            }
            username[i] = '\0';
            i = 0;
            for (; i < password_.size(); i++) {
                password[i] = password_[i];
            }
            password[i] = '\0';
        };
};

struct LoginResponse: public BaseMessage {
    bool response;
    int logintype;
    LoginResponse(int16_t id_, bool success_, int type_, int packet_id_):
        response(success_), logintype(type_) {
            id = id_;
            packet_id = packet_id_;
        };
};

struct FriendsMessage: public BaseMessage {
    char username[20];
    FriendsMessage(int16_t id_, std::string username_, int packet_id_):
        username() {
            int i = 0;
            id = id_;
            packet_id = packet_id_;
            for (; i < username_.size(); i++) {
                username[i] = username_[i];
            }
            username[i] = '\0';
        };
};

struct FriendsResponse: public BaseMessage {
    char friends[20];
    FriendsResponse(int16_t id_, std::string friends_, int packet_id_) {
            id = id_;
            int i = 0;
            for (; i < friends_.size(); i++) {
                friends[i] = friends_[i];
            }
            packet_id = packet_id_;
        };
};

struct AddFriendsMessage: public BaseMessage {
    char username[20];
    char friendName[20];

    AddFriendsMessage(int16_t id_, std::string username_, std::string friendName_, int packet_id_):
        username(), friendName() {
            int i = 0;
            id = id_;
            packet_id = packet_id_;
            for (; i < username_.size(); i++) {
                username[i] = username_[i];
            }
            username[i] = '\0';
            i = 0;
            for (; i < username_.size(); i++) {
                friendName[i] = friendName_[i];
            }
            friendName[i] = '\0';
        };
};

struct AddFriendsResponse: public BaseMessage {
    bool response;
    AddFriendsResponse(int16_t id_, bool success_, int packet_id_):
        response(success_) {
            id = id_;
            packet_id = packet_id_;
        };
};

struct RemoveFriendsMessage: public BaseMessage {
    char username[20];
    char friendName[20];

    RemoveFriendsMessage(int16_t id_, std::string username_, std::string friendName_, int packet_id_):
        username(), friendName() {
            int i = 0;
            id = id_;
            packet_id = packet_id_;
            for (; i < username_.size(); i++) {
                username[i] = username_[i];
            }
            username[i] = '\0';
            i = 0;
            for (; i < username_.size(); i++) {
                friendName[i] = friendName_[i];
            }
            friendName[i] = '\0';
        };
};

struct RemoveFriendsResponse: public BaseMessage {
    bool response;
    RemoveFriendsResponse(int16_t id_, bool success_, int packet_id_):
        response(success_) {
            id = id_;
            packet_id = packet_id_;
        };
};

struct ChatMessage: public BaseMessage {
    char name[20];
    char content[256];

    ChatMessage(int16_t id_, std::string name_, std::string content_, int packet_id_) {
        int i = 0;
        id = id_;
        packet_id = packet_id_;
        for (; i < content_.size(); i++) {
            content[i] = content_[i];
        }
        content[i] = '\0';
        i = 0;
        for (; i < name_.size(); i++) {
            name[i] = name_[i];
        }
        name[i] = '\0';
    };
};

struct DrawableSnapshot: public BaseMessage {
    entity_t entity;
    char data[1024];

    DrawableSnapshot(int16_t id_, entity_t entity_, std::string path, int packet_id_):
    entity(entity_) {
        int i = 0;
        id = id_;
        packet_id = packet_id_;
        for (; i < path.size(); i++) {
            data[i] = path[i];
        }
        data[i] = '\0';
    };
};

struct AnimatedDrawableSnapshot: public BaseMessage {
    /**
     * @brief entity to update the DrawableSnapshot for
     */
    entity_t entity;
    /**
     * @brief path to the desired asset
    */
    char _path[256];
    char _state[16];

    std::array<std::pair<char[16], std::pair<bool, std::pair<int, int>>>, 5> _anims;
    /**
     * @brief number of sprites on the x & y axis
     * 
     * (ex: {3, 4} for a 3 Sprites per line & 4 lines)
     * 
     */
    std::pair<int, int> _nbSprites;
    /**
     * @brief set of numbers representing the size of the sprites to load
     * 
     * (ex: {32, 14} for a 32x14 sprite)
     * 
     */
    std::pair<int, int> _spriteSize;
    /**
     * @brief set of numbers representing the size of the gapes between sprites
     * 
     * (ex: {3, 1} for a 3x1 gap)
     * 
     */
    std::pair<int, int> _gaps;
    /**
     * @brief set of numbers representing the size of the offset of the first sprite
     * 
     */
    std::pair<int, int> _firstOffset;
    /**
     * @brief set of numbers representing the indexes of the animation
     * 
     * (ex: {2, 0} to start with the 3rd sprite)
     * ! The second argument is not used yet !
     */
    std::pair<int, int> _currentIdx;

    AnimatedDrawableSnapshot &operator=(const AnimatedDrawableSnapshot &snapshot) {
        entity = snapshot.entity;
        _currentIdx = snapshot._currentIdx;
        _firstOffset = snapshot._firstOffset;
        _gaps = snapshot._gaps;
        _spriteSize = snapshot._spriteSize;
        _nbSprites = snapshot._nbSprites;
        for (int i = 0; i < snapshot._anims.size(); i++) {
            int j = 0;
            for (j = 0; snapshot._anims[i].first[j] != '\0'; j++) {
                _anims[i].first[j] = snapshot._anims[i].first[j];
            }
            _anims[i].first[j] = '\0';
            _anims[i].second = snapshot._anims[i].second;
        }
        int i = 0;
        for (i = 0; snapshot._state[i] != '\0'; i++) {
            _state[i] = snapshot._state[i];
        }
        _state[i] = '\0';
        for (i = 0; snapshot._path[i] != '\0'; i++) {
            _path[i] = snapshot._path[i];
        }
        _path[i] = '\0';
        return *this;
    };
    AnimatedDrawableSnapshot(
        int16_t id_,
        entity_t entity_,
        const std::string &path,
        std::pair<int, int> nbSprites,
        std::pair<int, int> spriteSize,
        std::pair<int, int> gaps,
        std::pair<int, int> firstOffset,
        std::pair<int, int> curretnIdx,
        animation_t anims_,
        const std::string &state,
        int packet_id_
    ) : _nbSprites(nbSprites), _spriteSize(spriteSize), _gaps(gaps), _firstOffset(firstOffset), _currentIdx(curretnIdx), entity(entity_) {
        int i = 0;
        int j = 0;

        id = id_;
        packet_id = packet_id_;
        for (; i < path.size(); i++) {
            _path[i] = path[i];
        }
        _path[i] = '\0';
        for (i = 0; i < state.size(); i++) {
            _state[i] = state[i];
        }
        _state[i] = '\0';
        for (auto &anim: anims_) {
            for (i = 0; i < anim.first.size(); i++) {
                _anims[j].first[i] = anim.first[i];
            }
            _anims[j].second = anim.second;
            ++j;
        }
        for (int k = j; k < _anims.size(); ++k)
            _anims[k].first[0] = '\0';
    };
};

struct AnimatedStateUpdateMessage: public BaseMessage {
    entity_t entity;
    char state[16];

    AnimatedStateUpdateMessage(int16_t id_, entity_t entity_, std::string state_, int packet_id_):
        entity(entity_) {
            int i = 0;
            
            id = id_;
            packet_id = packet_id_;
            for (; i < state_.size(); i++) {
                state[i] = state_[i];
            }
            state[i] = '\0';
        };
};

enum Stage {
    ONE,
    TWO,
    THREE,
};

enum inGameState {
    INGAME,
    INGAMEMENU,
};

struct Trophy {
    sf::Sprite sprite;
    sf::Texture texture;
};

struct HighScoreDisplay {
    sf::Text title;
    sf::Text name1;
    sf::Text score1;
    sf::Text name2;
    sf::Text score2;
    sf::Text name3;
    sf::Text score3;
    Trophy trophy1;
    Trophy trophy2;
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
        void receive();
        int recieve_position_snapshot_update(std::vector<char> &);
        std::vector<char> recieve_raw_data_from_client();
        int recieve_high_score(std::vector<char> &server_msg);
        int recieve_login_response(std::vector<char> &server_msg);
        int recieve_drawable_snapshot_update(std::vector<char> &server_msg);
        int recieve_animated_drawable_snapshot(std::vector<char> &server_msg);
        int recieve_animated_drawable_state_update(std::vector<char> &server_msg);
        int receive_friends_reponse(std::vector<char> &server_msg);
        int receive_add_friends_reponse(std::vector<char> &server_msg);
        int receive_remove_friends_reponse(std::vector<char> &server_msg);
        int receive_chat_event(std::vector<char> &server_msg);
        void createEnemy(std::pair<float, float> pos, std::pair<float, float> vel, const std::string &path_to_texture, std::pair<float, float> scale, int health, int damage);
        void displayScoreBoardMenu();

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
            {4, &Client::recieve_position_snapshot_update},
            {6, &Client::recieve_drawable_snapshot_update},
            {7, &Client::recieve_high_score},
            {8, &Client::recieve_login_response},
            {9, &Client::receive_friends_reponse},
            {10, &Client::receive_add_friends_reponse},
            {11, &Client::receive_remove_friends_reponse},
            {12, &Client::receive_chat_event},
            {13, &Client::recieve_animated_drawable_snapshot},
            {14, &Client::recieve_animated_drawable_state_update}
        };
        sf::Vector2i _mouse_position;
        sf::Text _mouse_position_text;
        HighScoreDisplay _highScoreDisplay;
        inGameState _state;
        int _packet_id = 0;
        std::vector<std::string> friendLists;
        std::mutex mtx;
};

#endif // CLIENT_HPP
