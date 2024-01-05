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

struct Friendship {
    std::string name;
    std::string id;
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
        void send_animated_drawable_snapshots_for_specific_player(entity_t entity);
        void send_animated_drawable_snapshot_to_all_players(entity_t entity);
        void send_animated_drawable_update_to_all_clients(entity_t entity, std::string state);
        void send_entity_drawable_to_all_players(entity_t entity);
        std::vector<char> recieve_raw_data_from_client();
        std::pair<int, int> get_position_change_for_event(entity_t entity, int event);
        int recieve_client_event(std::vector<char> &, entity_t);
        int recieve_connection_event(std::vector<char> &, entity_t);
        int recieve_disconnection_event(std::vector<char> &, entity_t);
        int recieve_packet_confirm(std::vector<char> &, entity_t);
        int receive_login_event(std::vector<char> &, entity_t);
        template <typename T>
        void send_data_to_all_clients(T& structure, std::vector<T>& packets_to_send);
        template <typename T>
        void send_data_to_client_by_entity(T& structure, entity_t entity);
        void sendPositionpacketsPeriodically();
        void connectToDB();
        HighScoreMessage getHighScore();
        void addHighScore(std::string name, int score);
        bool IsNameInBdd(std::string name);
        bool signUp(std::string name, std::string password);
        bool checkIfUserExist(std::string name, std::string password);
        std::string makePersonnalID();
        bool signIn(std::string name, std::string password);
        void addFriend(std::string name, std::string friendName);
        bool checkIfFriendshipExist(std::string name, std::string friendId);
        void removeFriend(std::string name, std::string friendName);
        void displayFriends(std::string name);
        Friendship getFriendsData(std::string id);
        void send_highscore_to_specific_client(entity_t);
        void send_all_entity_drawables_to_specific_player(entity_t player);
        template <typename T>
        void resend_packets(std::vector<T> &);
    private:
        std::vector<SnapshotPosition> _position_packets;
        std::vector<HighScoreMessage> _highscore_packets;
        std::vector<DrawableSnapshot> _drawable_packets;
        std::vector<AnimatedDrawableSnapshot> _animated_drawable_packets;
        std::vector<AnimatedStateUpdateMessage> _animated_drawable_update_packets;
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
            {6, &Server::receive_login_event}
        };
        std::string _event;
        std::thread _send_thread;
        sqlite3 *_db;
        int _highScore = 0;
        std::string _nameForHighScore = "";
};

#endif // SERVER_HPP
