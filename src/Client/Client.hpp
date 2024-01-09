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
    #define MAX_POSITION_MOVE_THRESHOLD 100
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
    #include "../Network.hpp"
    #include "../Timer.hpp"

struct data_struct {
    int id;
    sf::Event event;
    int package_id;

};

enum Stage {
    ONE,
    TWO,
    THREE,
};

enum inGameState {
    INGAME,
    INGAMEMENU,
    CHAT
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

struct ChatEntity {
    sf::RectangleShape _rectangle;
    std::vector<std::string> _chat;
    std::vector<sf::Text> _chatText;
    sf::Text _chatTitle;
    sf::RectangleShape _inputBox;
    std::string _input;
    sf::Text _chatTextInput;
    sf::Clock _clock;
};

class Client {
    typedef int (Client::*messageParserHandle)(std::vector<char>&);
    public:
        Client(std::string ip, int port, EventListener &listener, registry &ecs, std::mutex &mtx_);
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
        int recieve_scale_snapshot_update(std::vector<char> &);
        std::vector<char> recieve_raw_data_from_client();
        int recieve_death_event(std::vector<char> &server_msg);
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
        void handleInput(sf::Event &event);
        void manageCli();
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
        registry &_ecs;
        EventListener &_listener;
        Stage _stage;
        // content for enemys
        std::thread receiveThread;
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
            {14, &Client::recieve_animated_drawable_state_update},
            {15, &Client::recieve_scale_snapshot_update},
            {16, &Client::recieve_death_event}
        };
        sf::Vector2i _mouse_position;
        sf::Text _mouse_position_text;
        HighScoreDisplay _highScoreDisplay;
        inGameState _state;
        int _packet_id = 0;
        std::string friendLists;
        bool friendListFinish = false;
        ChatEntity _chatEntity;
        std::mutex &mtx;
        Timer shootTimer;
        Timer moveTimer;
        bool prgrmstop = false;
};

#endif // CLIENT_HPP
