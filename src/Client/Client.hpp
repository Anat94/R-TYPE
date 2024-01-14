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

/**
 * @brief check state of player in game (e.g. in chat, in highscore menu, playing, ...)
 * 
 */
enum inGameState {
    INGAME,
    INGAMEMENU,
    CHAT
};

/**
 * @brief Trophy item object
 * 
 */
struct Trophy {
    /**
     * @brief sprite for the trophy
     * 
     */
    sf::Sprite sprite;
    /**
     * @brief texture for the trophy
     * 
     */
    sf::Texture texture;
};

/**
 * @brief High Score Display
 * 
 */
struct HighScoreDisplay {
    /**
     * @brief Title of the menu
     * 
     */
    sf::Text title;
    /**
     * @brief name of the highest score
     * 
     */
    sf::Text name1;
    /**
     * @brief highest score
     * 
     */
    sf::Text score1;
    /**
     * @brief name of the 2nd highest score
     * 
     */
    sf::Text name2;
    /**
     * @brief 2nd highest score
     * 
     */
    sf::Text score2;
    /**
     * @brief name of the 3rd highest score
     * 
     */
    sf::Text name3;
    /**
     * @brief 3rd highest score
     * 
     */
    sf::Text score3;
    /**
     * @brief Trophy object 1
     * 
     */
    Trophy trophy1;
    /**
     * @brief Trophy object 2
     * 
     */
    Trophy trophy2;
};

/**
 * @brief Chat Entity
 * 
 */
struct ChatEntity {
    /**
     * @brief Rectangle englobing the chat
     * 
     */
    sf::RectangleShape _rectangle;
    /**
     * @brief vector of messages for the chat
     * 
     */
    std::vector<std::string> _chat;
    /**
     * @brief vector of chat texts
     * 
     */
    std::vector<sf::Text> _chatText;
    /**
     * @brief title of the chat
     * 
     */
    sf::Text _chatTitle;
    /**
     * @brief rectangle used for the input box
     * 
     */
    sf::RectangleShape _inputBox;
    /**
     * @brief input of the user
     * 
     */
    std::string _input;
    /**
     * @brief input of the user in SFML text format
     * 
     */
    sf::Text _chatTextInput;
    /**
     * @brief Clock for the chat
     * 
     */
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

        void displayTexts();

        /**
         * @brief Increase the level
         *
         */
        void addLevel() { _level++; }

        /**
         * @brief Increase the score
         *
         */
        void addScore() { _score++; }

        /**
         * @brief Decrease the lives
         *
         */
        void decreaseLives() { _lives--; }

        /**
         * @brief Increase the lives
         *
         */
        void increaseLives() { _lives++; }

        /**
         * @brief Get the level
         *
         * @return int
         */
        void setLevel(int level) { _level = level; }
        int manageEvent();
        void receive();
        int receive_position_snapshot_update(std::vector<char> &);
        int receive_scale_snapshot_update(std::vector<char> &);
        std::vector<char> receive_raw_data_from_client();
        int receive_death_event(std::vector<char> &server_msg);
        int receive_high_score(std::vector<char> &server_msg);
        int receive_login_response(std::vector<char> &server_msg);
        int receive_drawable_snapshot_update(std::vector<char> &server_msg);
        int receive_animated_drawable_snapshot(std::vector<char> &server_msg);
        int receive_animated_drawable_state_update(std::vector<char> &server_msg);
        int receive_friends_reponse(std::vector<char> &server_msg);
        int receive_add_friends_reponse(std::vector<char> &server_msg);
        int receive_remove_friends_reponse(std::vector<char> &server_msg);
        int receive_chat_event(std::vector<char> &server_msg);
        int recieve_room_creation_event(std::vector<char> &server_msg);
        int receive_room_join_event(std::vector<char> &server_msg);
        int receive_health_event(std::vector<char> &server_msg);
        int receive_score_event(std::vector<char> &server_msg);
        void displayScoreBoardMenu();
        void handleInput(sf::Event &event);
        entity_t init_new_entity(entity_t srvEntity);
        void manageCli();
        void initClass();
        entity_t get_entity_from_server_entity(entity_t srvEntity);
    private:
        /**
         * @brief message containing client event that is going to be sent to the server
         * 
         */
        EventMessage _send_structure;
        /**
         * @brief ASIO context
         * 
         */
        asio::io_context _io_context;
        /**
         * @brief client socket
         * 
         */
        udp::socket _socket;
        /**
         * @brief server endpoint
         * 
         */
        udp::endpoint _server_endpoint;
        /**
         * @brief username of the client
         * 
         */
        std::string _username;
        /**
         * @brief list of SFML events for which to send to the server
         * 
         */
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
        /**
         * @brief background entity for the moving background
         * 
         */
        entity_t _background;
        //Content for SFML
        /**
         * @brief SFML render window
         * 
         */
        sf::RenderWindow _window;
        /**
         * @brief SFML event
         * 
         */
        sf::Event _event;
        /**
         * @brief Global font used throughout the game display
         * 
         */
        sf::Font _font;
        /**
         * @brief Music played in the background
         * 
         */
        sf::Music _music;
        /**
         * @brief Score of the player
         * 
         */
        int _score;
        /**
         * @brief health of the player
         * 
         */
        int _lives;
        /**
         * @brief current level of the player
         * 
         */
        int _level;
        /**
         * @brief score text to be displayed on screen
         * 
         */
        sf::Text _score_text;
        /**
         * @brief health text to be displayed on screen
         * 
         */
        sf::Text _lives_text;
        /**
         * @brief level text to be displayed on screen
         * 
         */
        sf::Text _level_text;
        /**
         * @brief reference to the ECS Listener
         * 
         */
        EventListener &_listener;
        /**
         * @brief reference to the ECS
         * 
         */
        registry &_ecs;
        /**
         * @brief thread for reception of server messages
         * 
         */
        std::thread receiveThread;
        /**
         * @brief ids mapped to Client methods used to receive different server messages
         * 
         */
        std::map<int16_t, messageParserHandle> _messageParser = {
            {4, &Client::receive_position_snapshot_update},
            {6, &Client::receive_drawable_snapshot_update},
            {7, &Client::receive_high_score},
            {8, &Client::receive_login_response},
            {9, &Client::receive_friends_reponse},
            {10, &Client::receive_add_friends_reponse},
            {11, &Client::receive_remove_friends_reponse},
            {12, &Client::receive_chat_event},
            {13, &Client::receive_animated_drawable_snapshot},
            {14, &Client::receive_animated_drawable_state_update},
            {15, &Client::receive_scale_snapshot_update},
            {16, &Client::receive_death_event},
            {21, &Client::recieve_room_creation_event},
            {22, &Client::receive_room_join_event},
            {23, &Client::receive_health_event},
            {24, &Client::receive_score_event},
        };
        /**
         * @brief mouse position on screen
         * 
         */
        sf::Vector2i _mouse_position;
        /**
         * @brief mouse position text
         * 
         */
        bool spectator_mode = false;
        sf::Text _mouse_position_text;
        /**
         * @brief high score display structure
         * 
         */
        HighScoreDisplay _highScoreDisplay;
        /**
         * @brief state of the game
         * 
         */
        inGameState _state;
        /**
         * @brief next packet id available to attach to a packet to be sent on the server
         * 
         */
        int _packet_id = 0;
        /**
         * @brief friend list
         * 
         */
        std::string friendLists;
        /**
         * @brief represents if we are at the end of friend list sent by the server
         * 
         */
        bool friendListFinish = false;
        /**
         * @brief Chat entity
         * 
         */
        ChatEntity _chatEntity;
        /**
         * @brief reference to the mutex shared by the ECS and the Client
         * 
         */
        std::mutex &mtx;
        /**
         * @brief timer used to increase the interval during which a user can spam shoot
         * 
         */
        Timer shootTimer;
        /**
         * @brief timer used to create an interval between every movement
         * 
         */
        Timer moveTimer;
        /**
         * @brief room name of the client
         * 
         */
        std::string _room_name = "";
        /**
         * @brief indicates if the program is exiting
         * 
         */
        bool prgrmstop = false;
        /**
         * @brief indicates wether the user is logged in or not
         * 
         */
        bool _logged_in = false;
        /**
         * @brief vector containing packets received from the server
         * 
         */
        std::vector<int> _packets_received;
};

#endif // CLIENT_HPP
