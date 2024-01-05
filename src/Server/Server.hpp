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
            for (; i < password_.size(); i++) {
                password[i] = password_[i];
            }
            password[i] = '\0';
        };
};

struct LoginResponse: public BaseMessage {
    bool response;
    int logintype;
    LoginResponse(int16_t id_, bool success_, int _type, int packet_id_):
        response(success_), logintype(_type) {
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

struct Friendship {
    std::string name;
    std::string id;
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

class Server {
    typedef int (Server::*messageParserHandle)(std::vector<char>&, entity_t);
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
        int recieve_client_event(std::vector<char> &, entity_t);
        int recieve_connection_event(std::vector<char> &, entity_t);
        int recieve_disconnection_event(std::vector<char> &, entity_t);
        int recieve_packet_confirm(std::vector<char> &, entity_t);
        int receive_login_event(std::vector<char> &, entity_t);
        int receive_friend_event(std::vector<char> &, entity_t);
        int receive_add_friend_event(std::vector<char>&, entity_t);
        int receive_remove_friend_event(std::vector<char>&, entity_t);
        template <typename T>
        void send_data_to_all_clients(T& structure);
        template <typename T>
        void send_data_to_client_by_entity(T& structure, entity_t entity) {
            auto endpoint = _ecs.get_components<component::Endpoint>()[entity];
            if (!endpoint.has_value()) {
                std::cout << "INVALID ENDPOINT FOR ENTITY: " << entity << std::endl;
                return;
            }
            while (!can_mod) continue;
            _socket.send_to(asio::buffer(&structure, sizeof(structure)), endpoint->_endpoint);
        }
        void sendPositionpacketsPeriodically();
        void connectToDB();
        HighScoreMessage getHighScore();
        void addHighScore(std::string name, int score);
        bool IsNameInBdd(std::string name);
        bool signUp(std::string name, std::string password);
        bool checkIfUserExist(std::string name, std::string password);
        std::string makePersonnalID();
        bool signIn(std::string name, std::string password);
        bool addFriend(std::string name, std::string friendId);
        bool checkIfFriendshipExist(std::string name, std::string friendId);
        bool removeFriend(std::string name, std::string friendName);
        std::vector<Friendship> displayFriends(std::string name, entity_t player_entity);
        Friendship getFriendsData(std::string id);
        void send_highscore_to_specific_client(entity_t);
        void send_all_entity_drawables_to_specific_player(entity_t player);
        template <typename T>
        void resend_packets(std::vector<T> &);
    private:
        std::vector<SnapshotPosition> _position_packets;
        std::vector<HighScoreMessage> _highscore_packets;
        std::vector<DrawableSnapshot> _drawable_packets;
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
            {6, &Server::receive_login_event},
            {7, &Server::receive_friend_event},
            {8, &Server::receive_add_friend_event},
            {9, &Server::receive_remove_friend_event},
        };
        std::string _event;
        std::thread _send_thread;
        sqlite3 *_db;
        int _highScore = 0;
        std::string _nameForHighScore = "";
        bool can_mod = true;
};

#endif // SERVER_HPP
