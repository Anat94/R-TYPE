/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** main
*/

#ifndef NETWORK_HPP
    #define NETWORK_HPP
        #define MAX_BUF_SIZE 11024

#include <asio.hpp>
#include "../Ecs/Events.hpp"
#include "../Ecs/Components.hpp"
using asio::ip::udp;


/**
 * @brief Base Message, all network structures must inherit from this.
*/
struct BaseMessage {
    /**
     * @brief id of the message that is going to be sent through the network. Identifies the type of message.
    */
    int16_t id;
    /**
     * @brief packet id of the message, used to identify individual packets
    */
    int packet_id;
};

/**
 * @brief Position snapshot, encapsulates positional data of game entities to be sent over Network
*/
struct SnapshotPosition: public BaseMessage {
    /**
     * @brief Identifies the associated game entity for which position data is transmitted.
    */
    entity_t entity;
    /**
     * @brief Contains positional information (e.g., coordinates) of the entity.
    */
    component::Position data;

    /**
     * @brief Base constructor of the Position Snapshot structure
    */
    SnapshotPosition(): data(0, 0) {};
     /**
     * @brief Constructs SnapshotPosition with the following data
     * 
     * @param id_ id of the message that is going to be sent through the network. Identifies the type of message.
     * @param entity_ Identifies the associated game entity for which position data is transmitted.
     * @param data_ Contains positional information (e.g., coordinates) of the entity.
     * @param packet_id_ packet id of the message, used to identify individual packets
    */
    SnapshotPosition(int16_t id_, entity_t entity_, component::Position data_, int packet_id_):
    entity(entity_),  data(data_) {
        id = id_;
        packet_id = packet_id_;
    };
};

/**
 * @brief acts as a confirmation signal without additional payload.
*/
struct ConfirmationMessage: public BaseMessage {
};

/**
 * @brief Encapsulates game events transmitted from clients to the server.
*/
struct EventMessage: public BaseMessage {
    /**
     * @brief Contains the ID related to the event to be sent.
    */
    int event;
};

/**
 * @brief encapsulates the top 3 scores of all time from players
*/
struct HighScoreMessage: public BaseMessage {
    /**
     * @brief Top 1 score associated to the name1 player
    */
    int score1;
    /**
     * @brief Contains the username of the player who achieved score1.
    */
    char name1[20];
    /**
     * @brief Top 2 score associated to the name2 player
    */
    int score2;
    /**
     * @brief Contains the username of the player who achieved score2.
    */
    char name2[20];
    /**
     * @brief Top 3 score associated to the name3 player
    */
    int score3;
    /**
     * @brief Contains the username of the player who achieved score2.
    */
    char name3[20];
    /**
     * @brief Constructor initiating the following parameters
     * 
     * @param id_ id of the message that is going to be sent through the network. Identifies the type of message.
     * @param name1_ Contains the username of the player who achieved score1.
     * @param name2_ Contains the username of the player who achieved score2.
     * @param name3_ Contains the username of the player who achieved score3.
     * @param score1_ Top 1 score associated to the name1 player
     * @param score2_ Top 1 score associated to the name2 player
     * @param score3_ Top 1 score associated to the name3 player
     * @param packet_id_ packet id of the message, used to identify individual packets
    */
    HighScoreMessage(int16_t id_, std::string name1_, std::string name2_, std::string name3_,int score1_, int score2_, int score3_, int packet_id_):
        score1(score1_), score2(score2_), score3(score3_) {
            size_t i = 0;
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

/**
 * @brief encapsulates the login / signup details of the user
*/
struct LoginMessage: public BaseMessage {
    /**
     * @brief Username entered by the user.
    */
    char username[20];
    /**
     * @brief password entered by the user.
    */
    char password[20];
    /**
     * @brief type of the login: 0 = register, 1 = login.
    */
    int logintype;
    /**
     * @brief Login Message constructor, initiates the following parameter
     * 
     * @param id_ id of the message that is going to be sent through the network. Identifies the type of message.
     * @param username_ Username entered by the user.
     * @param password_ password entered by the user.
     * @param type_ type of the login: 0 = register, 1 = login.
     * @param packet_id_ packet id of the message, used to identify individual packets
    */
    LoginMessage(int16_t id_, std::string username_, std::string password_, int type_, int packet_id_):
        username(), password(), logintype(type_) {
            size_t i = 0;
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

/**
 * @brief encapsulates the login / signup response from the server to the user
*/
struct LoginResponse: public BaseMessage {
    /**
     * @brief response of the login request, 1 if successfull, 0 if not.
    */
    bool response;
    /**
     * @brief type of the login: 0 = register, 1 = login.
    */
    int logintype;
    /**
     * @brief Constructor initiating hte following parameters
     * 
     * @param id_ id of the message that is going to be sent through the network. Identifies the type of message.
     * @param response response of the login request, 1 if successfull, 0 if not.
     * @param logintype type of the login: 0 = register, 1 = login.
     * @param packet_id_ packet id of the message, used to identify individual packets
    */
    LoginResponse(int16_t id_, bool success_, int type_, int packet_id_):
        response(success_), logintype(type_) {
            id = id_;
            packet_id = packet_id_;
        };
};

/**
 * @brief is used as a request from the client to the server for a friend list
*/
struct FriendsMessage: public BaseMessage {
    /**
     * @brief Username of the user to fetch the friend list for.
    */
    char username[20];
    /**
     * @brief Constructor initiating the following parameters
     * 
     * @param id_ id of the message that is going to be sent through the network. Identifies the type of message.
     * @param username_ Username of the user to fetch the friend list for.
     * @param packet_id_ packet id of the message, used to identify individual packets
    */
    FriendsMessage(int16_t id_, std::string username_, int packet_id_):
        username() {
            size_t i = 0;
            id = id_;
            packet_id = packet_id_;
            for (; i < username_.size(); i++) {
                username[i] = username_[i];
            }
            username[i] = '\0';
        };
};

/**
 * @brief encapsulates a friend of the user's friend list
*/
struct FriendsResponse: public BaseMessage {
    /**
     * @brief username of the friend of the user's friend list.
    */
    char friends[20];
    /**
     * @brief constructor initiating the following parameters
     * 
     * @param id_ id of the message that is going to be sent through the network. Identifies the type of message.
     * @param friends_ username of the friend of the user's friend list.
     * @param packet_id_ packet id of the message, used to identify individual packets
    */
    FriendsResponse(int16_t id_, std::string friends_, int packet_id_) {
            id = id_;
            size_t i = 0;
            for (; i < friends_.size(); i++) {
                friends[i] = friends_[i];
            }
            packet_id = packet_id_;
        };
};

/**
 * @brief contains the information necessary to request a friend
*/
struct AddFriendsMessage: public BaseMessage {
    /**
     * Username of user.
    */
    char username[20];
    /**
     * Username of friend to remove.
    */
    char friendName[20];

    /**
     * @brief Constructor initializing following parameters
     * 
     * @param id_ id of the message that is going to be sent through the network. Identifies the type of message.
     * @param username_ Username of user.
     * @param friendName_ Username of friend to remove.
     * @param packet_id_ packet id of the message, used to identify individual packets
    */
    AddFriendsMessage(int16_t id_, std::string username_, std::string friendName_, int packet_id_):
        username(), friendName() {
            size_t i = 0;
            id = id_;
            packet_id = packet_id_;
            for (; i < username_.size(); i++) {
                username[i] = username_[i];
            }
            username[i] = '\0';
            i = 0;
            for (; i < friendName_.size(); i++) {
                friendName[i] = friendName_[i];
            }
            friendName[i] = '\0';
        };
};

/**
 * @brief encapsulates the friend request response from the server to the player
*/
struct AddFriendsResponse: public BaseMessage {
    /**
     * @brief response of the friend request, 1 if successfull, 0 if not.
    */
    bool response;
    /**
     * @brief Constructor initializing the following parameters
     * 
     * @param id_ id of the message that is going to be sent through the network. Identifies the type of message.
     * @param success_ response of the friend request, 1 if successfull, 0 if not.
     * @param packet_id_ packet id of the message, used to identify individual packets
    */
    AddFriendsResponse(int16_t id_, bool success_, int packet_id_):
        response(success_) {
            id = id_;
            packet_id = packet_id_;
        };
};

/**
 * @brief contains the information necessary to remove a friend from specified user
*/
struct RemoveFriendsMessage: public BaseMessage {
    /**
     * @brief Username of user.
    */
    char username[20];
    /**
     * @brief Username of friend to remove.
    */
    char friendName[20];

    /**
     * @brief constructor initializing the following parameters
     * 
     * @param id_ id of the message that is going to be sent through the network. Identifies the type of message.
     * @param username_ Username of user.
     * @param friendName_ Username of friend to remove.
     * @param packet_id_ packet id of the message, used to identify individual packets
    */
    RemoveFriendsMessage(int16_t id_, std::string username_, std::string friendName_, int packet_id_):
        username(), friendName() {
            size_t i = 0;
            id = id_;
            packet_id = packet_id_;
            for (; i < username_.size(); i++) {
                username[i] = username_[i];
            }
            username[i] = '\0';
            i = 0;
            for (; i < friendName_.size(); i++) {
                friendName[i] = friendName_[i];
            }
            friendName[i] = '\0';
        };
};

/**
 * @brief encapsulates the friend removal request response from the server to the player
*/
struct RemoveFriendsResponse: public BaseMessage {
    /**
     * @brief response of the friend removal request, 1 if successfull, 0 if not.
    */
    bool response;
    /**
     * @brief constructor initializing the following parameters
     * 
     * @param id_ id of the message that is going to be sent through the network. Identifies the type of message.
     * @param success_ response of the friend removal request, 1 if successfull, 0 if not.
     * @param packet_id_ packet id of the message, used to identify individual packets
    */
    RemoveFriendsResponse(int16_t id_, bool success_, int packet_id_):
        response(success_) {
            id = id_;
            packet_id = packet_id_;
        };
};

/**
 * @brief encapsulates the content and sender of a chat message
*/
struct ChatMessage: public BaseMessage {
    /**
     * @brief username of the sender.
    */
    char name[20];
    /**
     * @brief content of the message.
    */
    char content[256];

    /**
     * @brief constructor initializing the following parameters
     * 
     * @param id_ id of the message that is going to be sent through the network. Identifies the type of message.
     * @param name_ username of the sender.
     * @param content_ content of the message.
     * @param packet_id_ packet id of the message, used to identify individual packets
    */
    ChatMessage(int16_t id_, std::string name_, std::string content_, int packet_id_) {
        size_t i = 0;
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

/**
 * @brief encapsulates drawable data of game entities so that the client can correctly display the entity
*/
struct DrawableSnapshot: public BaseMessage {
    /**
     * @brief Identifies the associated game entity for which dawable data is transmitted.
    */
    entity_t entity = 0;
    /**
     * @brief Contains the path of the image to display for the entity (e.g., "assets/players/ship.png).
    */
    char data[1024];

    /**
     * @brief constructor initializing the following parameters
     * 
     * @param id_ id of the message that is going to be sent through the network. Identifies the type of message.
     * @param entity_ Identifies the associated game entity for which dawable data is transmitted.
     * @param path Contains the path of the image to display for the entity (e.g., "assets/players/ship.png).
     * @param packet_id_ packet id of the message, used to identify individual packets
    */
    DrawableSnapshot(int16_t id_, entity_t entity_, std::string path, int packet_id_):
    entity(entity_) {
        size_t i = 0;
        id = id_;
        packet_id = packet_id_;
        for (; i < path.size(); i++) {
            data[i] = path[i];
        }
        data[i] = '\0';
    };
};

/**
 * @brief encapsulates a scale snapshot for given entity
*/
struct ScaleSnapshot: public BaseMessage {
    /**
     * @brief Entity for which to set the scale
    */
    entity_t entity = 0;
    /**
     * @brief scale of the entity
    */
    component::Scale data;

    /**
     * @brief constructor initializing the following parameters
     * 
     * @param id_ id of the message that is going to be sent through the network. Identifies the type of message.
     * @param entity_ Entity for which to set the scale
     * @param data_ scale of the entity
     * @param packet_id_ packet id of the message, used to identify individual packets
    */
    ScaleSnapshot(int16_t id_, entity_t entity_, component::Scale data_, int packet_id_):
    entity(entity_), data(data_) {
        id = id_;
        packet_id = packet_id_;
    };
};

/**
 * @brief encapsulates a death event for given entity
*/
struct DeathEventMessage: public BaseMessage {
    /**
     * @brief Entity that died.
    */
    entity_t entity = 0;

    /**
     * @brief constructor initializing the following parameters
     * 
     * @param id_ id of the message that is going to be sent through the network. Identifies the type of message.
     * @param entity_ Entity that died.
     * @param packet_id_ packet id of the message, used to identify individual packets
    */
    DeathEventMessage(int16_t id_, entity_t entity_, int packet_id_):
    entity(entity_) {
        id = id_;
        packet_id = packet_id_;
    };
};

/**
 * @brief encapsulates the necessary information to initiate an animated drawable for a given entity
*/
struct AnimatedDrawableSnapshot: public BaseMessage {
    /**
     * @brief entity to initialize an AnimatedDrawableSnapshot for
     */
    entity_t entity = 0;
    /**
     * @brief Contains the path of the image to display for the entity (e.g., "assets/players/ship.png).
    */
    char _path[256];
    /**
     * @brief current state of the animated drawable to determin its animation (e.g., "idle").
    */
    char _state[16];

    /**
     * @brief an array of 5 elements maximum (5 * 25 bytes) representing the animations of the drawable.
    */
    std::array<std::pair<char[16], std::pair<bool, std::pair<int, int>>>, 5> _anims;
    /**
     * @brief number of sprites on the x & y axis
     * 
     * (ex: {3, 4} for a 3 Sprites per line & 4 lines)
     * 
     */
    std::pair<int, int> _nbSprites = {0, 0};
    /**
     * @brief set of numbers representing the size of the sprites to load
     * 
     * (ex: {32, 14} for a 32x14 sprite)
     * 
     */
    std::pair<int, int> _spriteSize = {0, 0};
    /**
     * @brief set of numbers representing the size of the gapes between sprites
     * 
     * (ex: {3, 1} for a 3x1 gap)
     * 
     */
    std::pair<int, int> _gaps = {0, 0};
    /**
     * @brief set of numbers representing the size of the offset of the first sprite
     * 
     */
    std::pair<int, int> _firstOffset = {0, 0};
    /**
     * @brief set of numbers representing the indexes of the animation
     * 
     * (ex: {2, 0} to start with the 3rd sprite)
     * ! The second argument is not used yet !
     */
    std::pair<int, int> _currentIdx = {0, 0};

    /**
     * @brief constructor initializing the following parameters
     * 
     * @param snapshot copies values from the snapshot parameter to the current structure members.
     * 
     * @returns reference to the modified structure
    */
    AnimatedDrawableSnapshot &operator=(const AnimatedDrawableSnapshot &snapshot) {
        entity = snapshot.entity;
        _currentIdx = snapshot._currentIdx;
        _firstOffset = snapshot._firstOffset;
        _gaps = snapshot._gaps;
        _spriteSize = snapshot._spriteSize;
        _nbSprites = snapshot._nbSprites;
        for (size_t i = 0; i < snapshot._anims.size(); i++) {
            int j = 0;
            for (j = 0; snapshot._anims[i].first[j] != '\0'; j++) {
                _anims[i].first[j] = snapshot._anims[i].first[j];
            }
            _anims[i].first[j] = '\0';
            _anims[i].second = snapshot._anims[i].second;
        }
        size_t i = 0;
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
    /**
     * @brief constructor initializing the following parameters
     * 
     * @param id_ id of the message that is going to be sent through the network. Identifies the type of message.
     * @param entity_ entity to initialize an AnimatedDrawableSnapshot for
     * @param path Contains the path of the image to display for the entity.
     * @param nbSprites number of sprites on the x & y axis
     * @param spriteSize set of numbers representing the size of the sprites to load
     * @param gaps set of numbers representing the size of the gapes between sprites
     * @param firstOffset set of numbers representing the size of the offset of the first sprite
     * @param curretnIdx set of numbers representing the indexes of the animation
     * @param anims_ an array of 5 elements maximum (5 * 25 bytes) representing the animations of the drawable.
     * @param state current state of the animated drawable to determin its animation
     * @param packet_id_ packet id of the message, used to identify individual packets
    */
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
    ) : entity(entity_), _nbSprites(nbSprites), _spriteSize(spriteSize), _gaps(gaps), _firstOffset(firstOffset), _currentIdx(curretnIdx) {
        size_t i = 0;
        size_t j = 0;

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
        for (size_t k = j; k < _anims.size(); ++k)
            _anims[k].first[0] = '\0';
    };
};

/**
 * @brief structure, derived from BaseMessage, encapsulates a state update of an animated drawable entity
*/
struct AnimatedStateUpdateMessage: public BaseMessage {
    /**
     * @brief Entity for which to update the animated drawable state.
    */
    entity_t entity = 0;
    /**
     * @brief updated state of the animated drawable (e.g. "idle" or "move up")
    */
    char state[16];

    /**
     * @brief constructor initializing the following parameters
     * 
     * @param id_ id of the message that is going to be sent through the network. Identifies the type of message.
     * @param entity_ Entity for which to update the animated drawable state.
     * @param state_ updated state of the animated drawable (e.g. "idle" or "move up")
     * @param packet_id_ packet id of the message, used to identify individual packets
    */
    AnimatedStateUpdateMessage(int16_t id_, entity_t entity_, std::string state_, int packet_id_):
        entity(entity_) {
            size_t i = 0;

            id = id_;
            packet_id = packet_id_;
            for (; i < state_.size(); i++) {
                state[i] = state_[i];
            }
            state[i] = '\0';
        };
};

/**
 * @brief contains the information necessary to create a room
*/
struct RoomCreationMessage: public BaseMessage {
    /**
     * @brief Username of host.
    */
    char username[20];
    /**
     * @brief Room Name
    */
    char room_name[32];

    /**
     * @brief constructor initializing the following parameters
     * 
     * @param id_ id of the message that is going to be sent through the network. Identifies the type of message.
     * @param username_ Username of host.
     * @param room_name_ Room Name
     * @param packet_id_ packet id of the message, used to identify individual packets
    */
    RoomCreationMessage(int16_t id_, std::string username_, std::string room_name_, int packet_id_)
    {
        size_t i = 0;

        id = id_;
        packet_id = packet_id_;
        for (; i < username_.size(); i++) {
            username[i] = username[i];
        }
        username[i] = '\0';
        for (i = 0; i < room_name_.size(); i++) {
            room_name[i] = room_name_[i];
        }
        room_name[i] = '\0';
    };
};

/**
 * @brief contains the information necessary to start the game for the user
*/
struct JoinGameMessage: public BaseMessage {
    /**
     * @brief Username of host.
    */
    char username[20];
    /**
     * @brief Room Name
    */
    char room_name[32];

    /**
     * @brief constructor initializing the following parameters
     * 
     * @param id_ id of the message that is going to be sent through the network. Identifies the type of message.
     * @param username_ Username of host.
     * @param room_name_ Room Name
     * @param packet_id_ packet id of the message, used to identify individual packets
    */
    JoinGameMessage(int16_t id_, std::string username_, std::string room_name_, int packet_id_)
    {
        size_t i = 0;

        id = id_;
        packet_id = packet_id_;
        for (; i < username_.size(); i++) {
            username[i] = username[i];
        }
        username[i] = '\0';
        for (i = 0; i < room_name_.size(); i++) {
            room_name[i] = room_name_[i];
        }
        room_name[i] = '\0';
    };
};

/**
 * @brief contains the information necessary to join a room
*/
struct RoomJoinMessage: public BaseMessage {
    /**
     * @brief Room Name
    */
    char room_name[32];

    /**
     * @brief constructor initializing the following parameters
     * 
     * @param id_ id of the message that is going to be sent through the network. Identifies the type of message.
     * @param room_name_ Room Name
     * @param packet_id_ packet id of the message, used to identify individual packets
    */
    RoomJoinMessage(int16_t id_, std::string room_name_, int packet_id_)
    {
        size_t i = 0;

        id = id_;
        packet_id = packet_id_;
        for (; i < room_name_.size(); i++) {
            room_name[i] = room_name_[i];
        }
        room_name[i] = '\0';
    };
};

/**
 * @brief structure to send health over network
 * 
 */
struct HealthMessage : public BaseMessage {
    /**
     * @brief Health of the player
    */
    int health;

    /**
     * @brief constructor initializing the following parameters
     *
     * @param id_ id of the message that is going to be sent through the network. Identifies the type of message
     * @param health_ Health of the player
     * @param packet_id_ packet id of the message, used to identify individual packets
    */
    HealthMessage(int16_t id_, int health_, int packet_id_)
    {
        id = id_;
        packet_id = packet_id_;
        health = health_;
    };
};

/**
 * @brief structure to send score over network
 * 
 */
struct ScoreMessage : public BaseMessage {
    /**
     * @brief Score of the player
    */
    int score;

    /**
     * @brief constructor initializing the following parameters
     *
     * @param id_ id of the message that is going to be sent through the network. Identifies the type of message
     * @param score_ Score of the player
     * @param packet_id_ packet id of the message, used to identify individual packets
    */
    ScoreMessage(int16_t id_, int score_, int packet_id_)
    {
        id = id_;
        packet_id = packet_id_;
        score = score_;
    };
};

#endif //NETWORK_HPP
