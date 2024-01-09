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

struct LoginMessage: public BaseMessage {
    char username[20];
    char password[20];
    int logintype;
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
            size_t i = 0;
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
            size_t i = 0;
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

struct DrawableSnapshot: public BaseMessage {
    entity_t entity = 0;
    char data[1024];

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

struct ScaleSnapshot: public BaseMessage {
    entity_t entity = 0;
    component::Scale data;

    ScaleSnapshot(int16_t id_, entity_t entity_, component::Scale data_, int packet_id_):
    entity(entity_), data(data_) {
        id = id_;
        packet_id = packet_id_;
    };
};

struct DeathEventMessage: public BaseMessage {
    entity_t entity = 0;

    DeathEventMessage(int16_t id_, entity_t entity_, int packet_id_):
    entity(entity_) {
        id = id_;
        packet_id = packet_id_;
    };
};

struct AnimatedDrawableSnapshot: public BaseMessage {
    /**
     * @brief entity to update the DrawableSnapshot for
     */
    entity_t entity = 0;
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

struct AnimatedStateUpdateMessage: public BaseMessage {
    entity_t entity = 0;
    char state[16];

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


#endif //NETWORK_HPP
