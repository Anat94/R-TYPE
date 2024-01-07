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
            int i = 0;
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
            int i = 0;
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

struct ChatMessage: public BaseMessage {
    char name[20];
    char content[256];

    ChatMessage(int16_t id_, std::string name_, std::string content_, int packet_id_) {
        int i = 0;
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


#endif //NETWORK_HPP
