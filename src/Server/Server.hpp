/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** main
*/

#ifndef SERVER_HPP
    #define SERVER_HPP
    #define MAX_BUF_SIZE 11024
    #include "../Ecs/Events.hpp"
    #include "../Ecs/ZipperIterator.hpp"
    #include <iostream>
    #include <thread>
    #include <chrono>
    #include <array>
    #include <asio.hpp>
    #include <SFML/Window.hpp>
    #include <SFML/Graphics.hpp>
    #include <SFML/System.hpp>
    #include <functional>
    #include <bsoncxx/json.hpp>
    #include <mongocxx/client.hpp>
    #include <mongocxx/instance.hpp>
    #include <mongocxx/uri.hpp>
    #include <mongocxx/options/client.hpp>
    #include <mongocxx/options/server_api.hpp>
    #include <bsoncxx/builder/stream/document.hpp>
    #include <bsoncxx/json.hpp>

using asio::ip::udp;

struct BaseMessage {
    int16_t id;
};

struct ConfirmationMessage: public BaseMessage {
    int packet_id;
};

struct SnapshotPosition: public BaseMessage {
    entity_t entity;
    component::Position data;
    int packet_id;

    SnapshotPosition(int16_t id_, entity_t entity_, component::Position data_, int packet_id_):
    entity(entity_),  data(data_), packet_id(packet_id_) {
        id = id_;
    };
};

struct EventMessage: public BaseMessage {
    sf::Event event;
    int packet_id;
};

class Server {
    typedef void (Server::*messageParserHandle)(std::vector<char>&, entity_t);
    public:
        Server(asio::io_context& service, int port, registry& ecs, rtype::event::EventListener& listener);
        ~Server();
        void recieve_from_client();
        entity_t get_player_entity_from_connection_address(udp::endpoint);
        entity_t connect_player(udp::endpoint player_endpoint);
        void send_position_snapshots_for_all_players();
        std::vector<char> recieve_raw_data_from_client();
        std::pair<int, int> get_position_change_for_event(entity_t entity, sf::Event event);
        void recieve_client_event(std::vector<char> &, entity_t);
        void recieve_connection_event(std::vector<char> &, entity_t);
        void recieve_disconnection_event(std::vector<char> &, entity_t);
        void recieve_packet_confirm(std::vector<char> &, entity_t);
        template <typename T>
        void send_data_to_all_clients(T& structure);
        void sendPositionPackagesPeriodically();
        std::vector<std::string> getDatabases();
        void getHighScore();
        void addHighScore(std::string name, int score);
        void connectToDB();
        void signUp(std::string name, std::string password);
        void signIn(std::string name, std::string password);
        std::string makePersonnalID();
        void addFriend(std::string name, std::string friendName);

    private:
        std::vector<SnapshotPosition> _position_packages;
        std::array<char, 1024> _buf;
        // asio::io_service &_service;
        asio::io_context &_service;
        udp::endpoint _remote_endpoint;
        std::vector<std::thread> _tpool;
        registry &_ecs;
        rtype::event::EventListener &_listener;
        asio::ip::udp::socket _socket;
        int _packet_id = 0;
        std::map<int16_t, messageParserHandle> _messageParser = {
            {1, &Server::recieve_client_event},
            {2, &Server::recieve_connection_event},
            {3, &Server::recieve_disconnection_event},
            {5, &Server::recieve_packet_confirm}
        };

        std::thread _send_thread;
        mongocxx::client _mongo_client;
        mongocxx::database _rtypeDb;
        mongocxx::database highscoreDb;
        int _highScore = 0;
        std::string _nameForHighScore = "";
};

#endif // SERVER_HPP
