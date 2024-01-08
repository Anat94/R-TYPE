/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** main
*/

#ifndef SERVER_HPP
    #define SERVER_HPP
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
    #include <mutex>
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
    #include "../Network.hpp"

struct Friendship {
    std::string name;
    std::string id;
};

class Server {
    typedef int (Server::*messageParserHandle)(std::vector<char>&, entity_t);
    public:
        Server(asio::io_context& service, int port, registry &ecs, EventListener &listener, std::mutex &mtx);
        ~Server();
        void recieve_from_client();
        entity_t get_player_entity_from_connection_address(udp::endpoint);
        entity_t connect_player(udp::endpoint player_endpoint);
        void send_death_event_to_all_players(entity_t entity);
        void send_all_scale_to_player(entity_t entity);
        void send_scale_to_all_players(entity_t entity);
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
        int receive_friend_event(std::vector<char> &, entity_t);
        int receive_add_friend_event(std::vector<char>&, entity_t);
        int receive_remove_friend_event(std::vector<char>&, entity_t);
        int receive_chat_event(std::vector<char>&, entity_t);
        template <typename T>
        void send_data_to_all_clients(T& structure, std::vector<T>& packets_to_send);
        template <typename T>
        void send_data_to_all_clients_except_me(T& structure);
        template <typename T>
        void send_data_to_client_by_entity(T& structure, entity_t entity) {
            std::cout << "GONNA SEND TO SPECIFC\n";
            auto endpoint = _ecs.get_components<component::Endpoint>()[entity];
            // while (!can_send) continue;
            // can_send = false;
            // while (!can_mod) continue;
            if (!endpoint.has_value()) {
                std::cout << "INVALID ENDPOINT FOR ENTITY: " << entity << std::endl;
                // can_send = true;
                return;
            }
            // can_mod = false;
            _socket.send_to(asio::buffer(&structure, sizeof(structure)), endpoint->_endpoint);
            // can_mod = true;
            // can_send = true;
            std::cout << "FINISHED SENDING TO SPECIFC\n";
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
        std::vector<DeathEventMessage> _death_packets;
        std::vector<SnapshotPosition> _position_packets;
        std::vector<HighScoreMessage> _highscore_packets;
        std::vector<DrawableSnapshot> _drawable_packets;
        std::vector<AnimatedDrawableSnapshot> _animated_drawable_packets;
        std::vector<AnimatedStateUpdateMessage> _animated_drawable_update_packets;
        std::vector<LoginResponse> _login_response_packets;
        std::vector<AddFriendsResponse> _add_friend_response_packets;
        std::vector<RemoveFriendsResponse> _remove_friend_response_packets;
        std::vector<FriendsResponse> _friends_response_packets;
        std::vector<ChatMessage> _chat_packets;
        std::vector<ScaleSnapshot> _scale_packets;
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
            {10, &Server::receive_chat_event},
        };
        std::string _event;
        std::thread _send_thread;
        sqlite3 *_db;
        int _highScore = 0;
        std::string _nameForHighScore = "";
        bool can_mod = true;
        bool can_send = true;
        bool can_read = true;
        std::mutex &mtx;
};

#endif // SERVER_HPP
