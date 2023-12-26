/*
** EPITECH PROJECT, 2023
** R-TYPE
** File description:
** database.cpp
*/

#include "Server.hpp"
#include <random>
#include <sqlite3.h>

static int callbackIsNameInBdd(void* data, int argc, char** argv, char** azColName) {
    bool* nameExists = static_cast<bool*>(data);
    *nameExists = true;
    return 0;
}

bool Server::IsNameInBdd(std::string name)
{
    const std::string tableName = "HighScore";
    std::string sql = "SELECT * FROM " + tableName + " WHERE name = '" + name + "';";
    char *zErrMsg = 0;
    bool nameExists = false;
    int rc;
    rc = sqlite3_exec(_db, sql.c_str(), callbackIsNameInBdd, &nameExists, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    return nameExists;
}

struct HighScore {
    std::string name;
    int score;
};

static int callbackGetHighScore(void* data, int argc, char** argv, char** azColName) {
    std::vector<HighScore>* results = static_cast<std::vector<HighScore>*>(data);
    results->push_back({argv[0], std::stoi(argv[1])});
    return 0;
}

void Server::getHighScore() {
    const std::string tableName = "HighScore";
    std::string sql = "SELECT * FROM " + tableName + " ORDER BY score DESC LIMIT 3;";
    char *zErrMsg = 0;
    std::vector<HighScore> results;
    int rc;
    rc = sqlite3_exec(_db, sql.c_str(), callbackGetHighScore, &results, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    for (const auto& highScore : results) {
        std::cout << "Name: " << highScore.name << ", Score: " << highScore.score << std::endl;
    }
}

void Server::addHighScore(std::string name, int score) {
    const std::string tableName = "HighScore";
    std::string sql;
    if (IsNameInBdd(name) == true) {
        sql = "UPDATE " + tableName + " SET score = " + std::to_string(score) + " WHERE name = '" + name + "';";
    } else {
        sql = "INSERT INTO " + tableName + " (name, score) VALUES ('" + name + "', " + std::to_string(score) + ");";
    }
    char *zErrMsg = 0;
    int rc;
    sqlite3_exec(_db, sql.c_str(), NULL, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "12SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Records created successfully\n");
    }
}

void Server::connectToDB() {
    int rc;
    rc = sqlite3_open("db/rtype.db", &_db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(_db));
        exit(84);
    } else {
        fprintf(stderr, "Opened database successfully\n");
    }
    // std::string sql12 = "DELETE FROM FRIENDS";
    // char *zErrMsg = 0;
    // rc = sqlite3_exec(_db, sql12.c_str(), NULL, 0, &zErrMsg);
    // if (rc != SQLITE_OK) {
    //     fprintf(stderr, "SQL error: %s\n", zErrMsg);
    //     sqlite3_free(zErrMsg);
    // } else {
    //     fprintf(stdout, "Records created successfully\n");
    // }
}

std::string Server::makePersonnalID()
{
    std::string str("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");

    std::random_device rd;
    std::mt19937 generator(rd());

    std::shuffle(str.begin(), str.end(), generator);
    std::cout << str.substr(0, 10) << std::endl;
    return str.substr(0, 10);
}

static int callbackIsUserExists(void* data, int argc, char** argv, char** azColName) {
    bool* nameExists = static_cast<bool*>(data);
    *nameExists = true;
    return 0;
}

bool Server::checkIfUserExist(std::string name, std::string password) {
    const std::string tableName = "USERS";
    std::string sql = "SELECT * FROM " + tableName + " WHERE name = '" + name + "';";
    char *zErrMsg = 0;
    bool nameExists = false;
    int rc;
    rc = sqlite3_exec(_db, sql.c_str(), callbackIsUserExists, &nameExists, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    if (nameExists == true) {
        return true;
    } else {
        return false;
    }
}

void Server::signUp(std::string name, std::string password) {
    const std::string tableName = "USERS";
    if (checkIfUserExist(name, password) == true) {
        std::cout << "User already exist" << std::endl;
        return;
    }
    std::string sql = "INSERT INTO " + tableName + " (id, name, password) VALUES ('" + makePersonnalID() +"', '" + name + "', '" + password + "');";
    char *zErrMsg = 0;
    int rc = sqlite3_exec(_db, sql.c_str(), NULL, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Records created successfully\n");
    }
}

static int callbackSignIn(void *data, int argc, char** argv, char** azColName) {
    bool* userExists = static_cast<bool*>(data);
    *userExists = true;
    return 0;
}

void Server::signIn(std::string name, std::string password) {
    const std::string tableName = "USERS";
    std::string sql = "SELECT * FROM " + tableName + " WHERE name = '" + name + "' AND password = '" + password + "';";
    char *zErrMsg = 0;
    bool userExists = false;
    int rc;
    rc = sqlite3_exec(_db, sql.c_str(), callbackSignIn, &userExists, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    if (userExists == true)
        std::cout << "User succesfully connected" << std::endl;
    else
        std::cout << "Bad credentials" << std::endl;
}

static int callbackCheckFriendship(void *data, int argc, char** argv, char** azColName) {
    bool* friendshipExists = static_cast<bool*>(data);
    *friendshipExists = true;
    return 0;
}

bool Server::checkIfFriendshipExist(std::string name, std::string friendId) {
    const std::string tableName = "FRIENDS";
    std::string sql = "SELECT * FROM " + tableName + " WHERE name = '" + name + "' AND friendId = '" + friendId + "';";
    bool exist = false;
    char *zErrMsg = 0;
    int rc = sqlite3_exec(_db, sql.c_str(), callbackCheckFriendship, &exist, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    if (exist == true)
        return true;
    else
        return false;
}

void Server::addFriend(std::string name, std::string friendId) {
    if (checkIfFriendshipExist(name, friendId) == true) {
        std::cout << "Friendship already exist" << std::endl;
        return;
    }
    const std::string tableName = "FRIENDS";
    std::string sql = "INSERT INTO " + tableName + "(name, friendID) VALUES ( '" + name +"', '" + friendId + "');";
    char *zErrMsg = 0;
    int rc = sqlite3_exec(_db, sql.c_str(), NULL, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    std::cout << "Friend added" << std::endl;
}

void Server::removeFriend(std::string name, std::string friendName) {
    const std::string tableName = "FRIENDS";
    std::string sql = "DELETE FROM " + tableName + " WHERE name = '" + name + "' AND friendId = '" + friendName + "';";
    char *zErrMsg = 0;
    int rc = sqlite3_exec(_db, sql.c_str(), NULL, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    std::cout << "Friend removed" << std::endl;
}

static int callbackGetFriendsData(void *data, int argc, char** argv, char** azColName) {
    Friendship* myFriend = static_cast<Friendship*>(data);
    myFriend->name = argv[0];
    myFriend->id = argv[1];
    return 0;
}

Friendship Server::getFriendsData(std::string id)
{
    const std::string tableName = "USERS";
    std::string sql = "SELECT * FROM " + tableName + " WHERE id = '" + id + "';";
    char *zErrMsg = 0;
    Friendship myFriend;
    int rc = sqlite3_exec(_db, sql.c_str(), callbackGetFriendsData, &myFriend, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    return myFriend;
}

struct CallbackData {
    Server* server;
    std::vector<Friendship>* friends;
};

static int callbackDisplayFriends(void *data, int argc, char** argv, char** azColName) {
    CallbackData* callbackData = static_cast<CallbackData*>(data);
    callbackData->friends->push_back(callbackData->server->getFriendsData(argv[1]));
    return 0;
}

void Server::displayFriends(std::string name) {
    const std::string tableName = "FRIENDS";
    std::string sql = "SELECT * FROM " + tableName + " WHERE name = '" + name + "';";
    char *zErrMsg = 0;
    std::vector<Friendship> friends;
    CallbackData callbackData = { this, &friends };
    int rc = sqlite3_exec(_db, sql.c_str(), callbackDisplayFriends, &callbackData, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    for (const auto& friendData : friends) {
        std::cout << "Name: " << friendData.name << ", ID: " << friendData.id << std::endl;
    }
}
