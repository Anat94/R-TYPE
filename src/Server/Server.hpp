/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** main
*/

#ifndef SERVER_HPP
    #define SERVER_HPP
    #define MAX_BUF_SIZE 11024
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
    #include "../Ecs/Events.hpp"
    #include "../Ecs/ZipperIterator.hpp"
    #include <iostream>
    #include <thread>
    #include <chrono>
    #include <array>
    #include <asio.hpp>
    #include <functional>
    #include "../Ecs/Events.hpp"
    #include "../Ecs/ZipperIterator.hpp"
    #include "../Ecs/Systems/CollisionSystem.hpp"
    #include "../Ecs/Systems/ControlSystem.hpp"
    #include "../Ecs/Systems/PositionSystem.hpp"
    #include "../Ecs/Systems/ResetOnMoveSystem.hpp"
    #include <sqlite3.h>

using asio::ip::udp;

struct BaseMessage {
    int16_t id;
    int packet_id;
};

struct ConfirmationMessage: public BaseMessage {
};

struct SnapshotPosition: public BaseMessage {
    entity_t entity;
    component::Position data;

    SnapshotPosition(int16_t id_, entity_t entity_, component::Position data_, int packet_id_):
    entity(entity_),  data(data_) {
        id = id_;
        packet_id = packet_id_;
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
    HighScoreMessage(int16_t id_, std::string name1_, std::string name2_, std::string name3_,int score1_, int score2_, int score3_, int packet_id_):
        score1(score1_), score2(score2_), score3(score3_) {
            int i = 0;
            id = id_;
            packet_id = packet_id_;
            for (; i < name1_.size(); i++) {
                name1[i] = name1_[i];
            }
            name1[i] = '\0';
            i = 0;
            for (; i < name2_.size(); i++) {
                name2[i] = name2_[i];
            }
            name2[i] = '\0';
            i = 0;
            for (; i < name3_.size(); i++) {
                name3[i] = name3_[i];
            }
            name3[i] = '\0';
        };
};

struct Friendship {
    std::string name;
    std::string id;
};

class Server {
    typedef void (Server::*messageParserHandle)(std::vector<char>&, entity_t);
    public:
        Server(asio::io_context& service, int port, registry &ecs, EventListener &listener);
        ~Server();
        void recieve_from_client();
        entity_t get_player_entity_from_connection_address(udp::endpoint);
        entity_t connect_player(udp::endpoint player_endpoint);
        void send_position_snapshots_for_all_players();
        void send_entity_drawable_to_all_players(entity_t entity);
        std::vector<char> recieve_raw_data_from_client();
        std::pair<int, int> get_position_change_for_event(entity_t entity, int event);
        void recieve_client_event(std::vector<char> &, entity_t);
        void recieve_connection_event(std::vector<char> &, entity_t);
        void recieve_disconnection_event(std::vector<char> &, entity_t);
        void recieve_packet_confirm(std::vector<char> &, entity_t);
        template <typename T>
        void send_data_to_all_clients(T& structure);
        template <typename T>
        void send_data_to_client_by_entity(T& structure, entity_t entity);
        void sendPositionPackagesPeriodically();
        void connectToDB();
        HighScoreMessage getHighScore();
        void addHighScore(std::string name, int score);
        bool IsNameInBdd(std::string name);
        void signUp(std::string name, std::string password);
        bool checkIfUserExist(std::string name, std::string password);
        std::string makePersonnalID();
        void signIn(std::string name, std::string password);
        void addFriend(std::string name, std::string friendName);
        bool checkIfFriendshipExist(std::string name, std::string friendId);
        void removeFriend(std::string name, std::string friendName);
        void displayFriends(std::string name);
        Friendship getFriendsData(std::string id);
        void send_highscore_to_specific_client(entity_t);
        void send_all_entity_drawables_to_specific_player(entity_t player);
    private:
        std::vector<SnapshotPosition> _position_packages;
        std::vector<HighScoreMessage> _highscore_packages;
        std::vector<DrawableSnapshot> _drawable_packages;
        std::array<char, 1024> _buf;
        // asio::io_service &_service;
        asio::io_context &_service;
        udp::endpoint _remote_endpoint;
        std::vector<std::thread> _tpool;
        registry &_ecs;
        EventListener &_listener;
        asio::ip::udp::socket _socket;
        int _packet_id = 0;
        std::map<int16_t, messageParserHandle> _messageParser = {
            {1, &Server::recieve_client_event},
            {2, &Server::recieve_connection_event},
            {3, &Server::recieve_disconnection_event},
            {5, &Server::recieve_packet_confirm},
        };
        std::string _event;
        std::thread _send_thread;
        sqlite3 *_db;
        int _highScore = 0;
        std::string _nameForHighScore = "";
};

#endif // SERVER_HPP
