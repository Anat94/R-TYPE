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
    #include "../Timer.hpp"

/**
 * @brief Friendship object
 * 
 */
struct Friendship {
    /**
     * @brief name of the friend
     * 
     */
    std::string name;
    /**
     * @brief id of the friend
     * 
     */
    std::string id;
};

/**
 * @brief Server system, used in the ECS to run the game and send information to clients
 * 
 */
class Server: public ISystems {
    typedef int (Server::*messageParserHandle)(std::vector<char>&, entity_t);
    public:
        /**
         * @brief Construct a new Server:: Server object
         *
         * @param service The server's io_context
         * @param port The port to listen to
         * @param ecs The server's registry
         * @param listener The server's event listener
         * @param mtx_ The server's mutex
         */
        Server(asio::io_context& service, int port, registry &ecs, EventListener &listener, std::mutex &mtx);
        /**
         * @brief Destroy the Server object
         * 
         */
        ~Server();

        /**
         * @brief receive info over udp network from client
         *
         */
        void receive_from_client();
        /**
         * @brief operator that gets called when the ECS launches systems
         * 
         * @param dra drawables
         * @param scl scales
         * @param pos position
         * @param edp endpoints
         */
        void operator()(sparse_array<component::AnimatedDrawable> &dra, sparse_array<component::Scale> &scl, sparse_array<component::Position> &pos, sparse_array<component::Endpoint> &edp);
        /**
         * @brief get player entity from connection address
         *
         * @param endpoint endpoint of the player
         * @return entity_t the player's entity
         * @return -1 if not found
        */
        entity_t get_player_entity_from_connection_address(udp::endpoint);
        /**
         * @brief Connect a player to the server
         *
         * @param data The player's data
         * @param username The player's username
         * @param room_name The player's room name
         * @return entity_t The player's entity
         */
        entity_t connect_player(udp::endpoint player_endpoint, std::string username, std::string room_name);
        /**
         * @brief send death event to all players
         *
         * @param entity entity to send
         * @param edp endpoint component
         */
        void send_death_event_to_all_players(entity_t entity, sparse_array<component::Endpoint> &edp);
        /**
         * @brief Send all scale to player
         *
         * @param entity The entity
         */
        void send_all_scale_to_player(entity_t entity);
        /**
         * @brief Send all scales in the player's room, to the player 
         *
         * @param entity The entity
         */
        void send_all_scale_to_player_by_room(entity_t entity);
        /**
         * @brief Send scale to all client
         *
         * @param entity The entity
         * @param scl scale component
         * @param edp endpoint component
         */
        void send_scale_to_all_players(entity_t entity, sparse_array<component::Scale> &scl, sparse_array<component::Endpoint> &edp);
        /**
         * @brief Send position snapshots for all players
         *
         * @param pos position component
         * @param edp endpoint component
         */
        void send_position_snapshots_for_all_players(sparse_array<component::Position> &pos, sparse_array<component::Endpoint> &edp);
        void send_health_to_specific_client(sparse_array<component::Endpoint> &edp);
        void send_score_to_specific_client(sparse_array<component::Endpoint> &edp);
        /**
         * @brief send animated drawable snapshot to specific player
         * 
         * @param entity entity to send
         * @param dra all animated drawables
         */
        void send_animated_drawable_snapshots_for_specific_player(entity_t entity, sparse_array<component::AnimatedDrawable> dra);
        /**
         * @brief send animated drawable update to all client
         *
         * @param entity entity to send
         * @param dra drawable to send
         * @param edp endpoint component
         */
        void send_animated_drawable_snapshot_to_all_players(entity_t entity, sparse_array<component::AnimatedDrawable> &dra, sparse_array<component::Endpoint> &edp);
        /**
         * @brief send animated drawable update to all clients
         *
         * @param entity entity to send
         * @param state state to send
         * @param edp endpoint component
         */
        void send_animated_drawable_update_to_all_clients(entity_t entity, std::string state, sparse_array<component::Endpoint> &edp);
        /**
         * @brief send drawable to all clients
         *
         * @param entity entity to send
         * @param dra all drawables
         * @param edp all endpoints
         */
        void send_entity_drawable_to_all_players(entity_t entity, sparse_array<component::Drawable> &dra, sparse_array<component::Endpoint> &edp);
        /**
         * @brief receive data from client
         *
         * @return std::vector<char>  the data received
         */
        std::vector<char> receive_raw_data_from_client();
        /**
         * @brief get position change for event
         *
         * @param entity the entity
         * @param event the event
         * @return std::pair<int, int> the position change
         */
        std::pair<int, int> get_position_change_for_event(entity_t entity, int event);
        /**
         * @brief receive client event
         *
         * @param client_msg the client message
         * @param player_entity the player entity
         * @return int 0 on success
         * @return int -1 on failure
         */
        int receive_client_event(std::vector<char> &, entity_t);
        /**
         * @brief receive connection event
         *
         * @param client_msg the client message
         * @param player_entity the player entity
         * @return int 0 on success
         */
        int receive_connection_event(std::vector<char> &, entity_t);
        /**
         * @brief receive disconnection event
         *
         * @param client_msg the client message
         * @param player_entity the player entity
         * @return int 0 on success
         */
        int receive_disconnection_event(std::vector<char> &, entity_t);
        /**
         * @brief receive packet confirm
         *
         * @param client_msg the client message
         * @param _ the entity
         * @return int 0
         */
        int receive_packet_confirm(std::vector<char> &, entity_t);
        /**
         * @brief receive login event
         *
         * @param client_msg the client message
         * @param player_entity the player entity
         * @return int 0 on success
         */
        int receive_login_event(std::vector<char> &, entity_t);
        /**
         * @brief receive friend event
         *
         * @param client_msg the client message
         * @param player_entity the player entity
         * @return int 0 on success
         * @return int -1 on failure
         */
        int receive_friend_event(std::vector<char> &, entity_t);
        /**
        * @brief receive add friend event
        *
        * @param client_msg message from client
        * @param player_entity the player entity
        * @return int 0 on success
        * @return int -1 on failure
        */
        int receive_add_friend_event(std::vector<char>&, entity_t);
        /**
         * @brief receive remove friend event
         *
         * @param client_msg message from client
         * @param player_entity the player entity
         * @return int 0 on success
         * @return int -1 on failure
         */
        int receive_remove_friend_event(std::vector<char>&, entity_t);
        /**
         * @brief receive chat event
         *
         * @param client_msg message from client
         * @param player_entity the player entity
         * @return int 0 on success
         * @return int -1 on failure
         */
        int receive_chat_event(std::vector<char>&, entity_t);
        /**
         * @brief receive an event of room creation from the client
         * 
         * @param client_msg raw client message unparsed.
         * @param _ 
         * @return packet id of the received message
         */
        int receive_room_creation_event(std::vector<char>&, entity_t);
        /**
         * @brief receive an event of room joining from the client
         * 
         * @param client_msg raw client message unparsed.
         * @param _ 
         * @return packet id of the received message
         */
        int receive_room_join_event(std::vector<char>&, entity_t);
        /**
         * @brief send data to all clients
         *
         * @tparam T the type of the data
         * @param structure the data
         * @param packets_to_send the packets to send
         * @param edp endpoint component
         */
        template <typename T>
        void send_data_to_all_clients(T& structure, std::vector<T>& packets_to_send, sparse_array<component::Endpoint> &edp);

        /**
         * @brief send data to all clients except me
         *
         * @tparam T the type of the data
         * @param structure the data
         * @param entity the entity
         */
        template <typename T>
        void send_data_to_all_clients_except_me(T& structure, sparse_array<component::Endpoint> &edp);
        /**
         * @brief send data to all clients by their room
         *
         * @tparam T the type of the data
         * @param structure the data
         * @param packets_to_send the packets to send
         * @param edp endpoint component
         * @param rms all rooms
         * @param room room to send data to
        */
        template <typename T>
        void send_data_to_all_clients_by_room(T& structure, std::vector<T>& packets_to_send, sparse_array<component::Endpoint> &edp, sparse_array<component::Room> &rms, std::string room);
        /**
         * @brief send data to specific client
         *
         * @tparam T structure type
         * @param structure structure to send
         * @param entity entity to send to
         */
        template <typename T>
        void send_data_to_client_by_entity(T& structure, entity_t entity) {
            auto &endpoint = _ecs.get_components<component::Endpoint>()[entity];
            if (!endpoint.has_value()) {
                std::cout << "INVALID ENDPOINT FOR ENTITY: " << entity << std::endl;
                return;
            }
            _resend_packets_endpoints[structure.packet_id] = endpoint->_endpoint;
            _socket.send_to(asio::buffer(&structure, sizeof(structure)), endpoint->_endpoint);
        }
        /**
         * @brief Connect to database
         *
         */
        void connectToDB();
        /**
         * @brief Get the High Score on db
         *
         * @return HighScoreMessage high score message
         */
        HighScoreMessage getHighScore();
        /**
         * @brief Callback add get high score
         *
         * @param data data return of the request
         * @param argc number of column
         * @param argv data of the column
         * @param azColName name of the column
         * @return int 0
         */
        void addHighScore(std::string name, int score);
        /**
         * @brief Check if name is in bdd
         *
         * @param name name to check
         * @return true name is in bdd
         * @return false name is not in bdd
         */
        bool IsNameInBdd(std::string name);
        /**
         * @brief Callback check if user exist
         *
         * @param data data return of the request
         * @param argc number of column
         * @param argv data of the column
         * @param azColName name of the column
         * @return int 0
         */
        bool signUp(std::string name, std::string password);
        /**
         * @brief Check if user exist
         *
         * @param name name of the user
         * @param password password of the user
         * @return true user exist
         * @return false user doesn't exist
         */
        bool checkIfUserExist(std::string name, std::string password);
        /**
         * @brief make personnal ID
         *
         * @return std::string personnal ID
         */
        std::string makePersonnalID();
        /**
         * @brief Signin the user
         *
         * @param name name of the user
         * @param password password of the user
         * @return true user signed in
         * @return false user does not signed in
         */
        bool signIn(std::string name, std::string password);
        /**
         * @brief add friends
         *
         * @param name your name
         * @param friendId name of the friend
         * @return true if success
         * @return false if failed
         */
        bool addFriend(std::string name, std::string friendId);
        /**
         * @brief Check if friendship exist
         *
         * @param name name of the user
         * @param friendId name of the friend
         * @return true friendship exist
         * @return false friendship does not exist
         */
        bool checkIfFriendshipExist(std::string name, std::string friendId);
        /**
         * @brief Remove friends
         *
         * @param name your name
         * @param friendId name of the friend
         * @return true if success
         * @return false if failed
         */
        bool removeFriend(std::string name, std::string friendName);
        /**
         * @brief Display friends
         *
         * @param name name of the user
         * @return std::vector<std::string> list of friends
         */
        std::vector<std::string> displayFriends(std::string name, entity_t player_entity);
        /**
         * @brief Send highscore to specific client
         *
         * @param new_player The player's entity
         */
        void send_highscore_to_specific_client(entity_t);
        /**
         * @brief send all entity drawables to specific player
         *
         * @param player the player entity
         */
        void send_all_entity_drawables_to_specific_player(entity_t player);
        /**
         * @brief send animated drawables in a player's room to a specific player
         * 
         * @param entity player to send the drawables dto
         * @param dra all animated drawables
         */
        void send_animated_drawable_snapshots_for_specific_player_by_room(entity_t entity, sparse_array<component::AnimatedDrawable> dra);
        /**
         * @brief send all the drawables in a player's room, to a specific player
         * 
         * @param player player to send drawables to
         */
        void send_all_entity_drawables_to_specific_player_by_room(entity_t player);
        template <typename T>
        /**
         * @brief resend packets
         *
         * @tparam T the type of the data
         * @param packets the packets to resend
         * @param edp endpoint component
         */
        void resend_packets(std::vector<T> &, sparse_array<component::Endpoint> &);
    private:
        /**
         * @brief death packets to resend through network if not confirmed
         * 
         */
        std::vector<DeathEventMessage> _death_packets;
        /**
         * @brief position packets to resend through network if not confirmed
         * 
         */
        std::vector<SnapshotPosition> _position_packets;
        /**
         * @brief highscore packets to resend through network if not confirmed
         * 
         */
        std::vector<HighScoreMessage> _highscore_packets;
        /**
         * @brief drawable packets to resend through network if not confirmed
         * 
         */
        std::vector<DrawableSnapshot> _drawable_packets;
        /**
         * @brief animated drawable packets to resend through network if not confirmed
         * 
         */
        std::vector<AnimatedDrawableSnapshot> _animated_drawable_packets;
        /**
         * @brief animated drawable state update packets to resend through network if not confirmed
         * 
         */
        std::vector<AnimatedStateUpdateMessage> _animated_drawable_update_packets;
        /**
         * @brief login response packets to resend through network if not confirmed
         * 
         */
        std::vector<LoginResponse> _login_response_packets;
        /**
         * @brief friend request response packets to resend through network if not confirmed
         * 
         */
        std::vector<AddFriendsResponse> _add_friend_response_packets;
        /**
         * @brief friend removal response packets to resend through network if not confirmed
         * 
         */
        std::vector<RemoveFriendsResponse> _remove_friend_response_packets;
        /**
         * @brief friend response packets to resend through network if not confirmed
         * 
         */
        std::vector<FriendsResponse> _friends_response_packets;
        /**
         * @brief chat message packets to resend through network if not confirmed
         * 
         */
        std::vector<ChatMessage> _chat_packets;
        /**
         * @brief scale packets to resend through network if not confirmed
         * 
         */
        std::vector<ScaleSnapshot> _scale_packets;
        /**
         * @brief room creation packets to resend through network if not confirmed
         * 
         */
        std::vector<RoomCreationMessage> _room_creation_packets;
        /**
         * @brief room join packets to resend through network if not confirmed
         * 
         */
        std::vector<RoomJoinMessage> _room_join_packets;
        /**
         * @brief health packets to resend through network if not confirmed
         * 
         */
        std::vector<HealthMessage> _health_packets_to_send;
        /**
         * @brief score packets to resend through network if not confirmed
         * 
         */
        std::vector<ScoreMessage> _score_packets_to_send;
        /**
         * @brief endpoints associated to packet_ids to know what endpoint sent what packet at the moment of the resend
         * 
         */
        std::unordered_map<int, udp::endpoint> _resend_packets_endpoints;
        // asio::io_service &_service;
        /**
         * @brief ASIO contect service
         * 
         */
        asio::io_context &_service;
        /**
         * @brief ASIO remote UDP endpoint, used to detect from whom we received a message
         * 
         */
        udp::endpoint _remote_endpoint;
        /**
         * @brief reference to the ECS
         * 
         */
        registry &_ecs;
        /**
         * @brief reference to the ECS event listener
         * 
         */
        EventListener &_listener;
        /**
         * @brief Server UDP socket
         * 
         */
        asio::ip::udp::socket _socket;
        /**
         * @brief newly available packet_id to use for sending packets with Unique IDs, is auto-incremented
         * 
         */
        int _packet_id = 0;
        /**
         * @brief client message parser, associates message IDs to receive methods
         * 
         */
        std::map<int16_t, messageParserHandle> _messageParser = {
            {1, &Server::receive_client_event},
            {2, &Server::receive_connection_event},
            {3, &Server::receive_disconnection_event},
            {5, &Server::receive_packet_confirm},
            {12, &Server::receive_chat_event},
            {17, &Server::receive_login_event},
            {18, &Server::receive_friend_event},
            {19, &Server::receive_add_friend_event},
            {20, &Server::receive_remove_friend_event},
            {21, &Server::receive_room_creation_event},
            {22, &Server::receive_room_join_event},
        };
        /**
         * @brief map of rooms associated to their respective host's username
         * 
         */
        std::unordered_map<std::string, std::string> _lobbies;
        /**
         * @brief database
         * 
         */
        sqlite3 *_db;
        /**
         * @brief highscore of all time
         * 
         */
        int _highScore = 0;
        /**
         * @brief name of the player that attained that high score
         * 
         */
        std::string _nameForHighScore = "";
        /**
         * @brief mutex reference of the main mutex shared between ECS & Server
         * 
         */
        std::mutex &mtx;
        /**
         * @brief Keeps track of the registered animated drawables, used to send to clients when a new animated drawable is detected ig
         * 
         */
        std::vector<entity_t> animatedDrawableRegistered = {};
        /**
         * @brief Keeps track of the alive entities, used to send to clients when dead entity is detected ig
         * 
         */
        std::vector<entity_t> entitiesAlive = {};
        /**
         * @brief Keeps track of the registered scales, used to send to clients when a new scale is detected ig
         * 
         */
        std::vector<entity_t> scalesRegistered = {};
        /**
         * @brief timer to optimize the execution of the packet resend, and periodical position sending
         * 
         */
        Timer timer;
        /**
         * @brief used to calculate how many 100 milliseconds have went by since last resend.
         * 
         */
        int resend_counter = 0;
};

#endif // SERVER_HPP
