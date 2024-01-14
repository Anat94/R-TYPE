/*
** EPITECH PROJECT, 2023
** R-TYPE
** File description:
** database.cpp
*/

#include "Server.hpp"
#include <random>
#include <sqlite3.h>

/**
 * @brief Callback check if name is in bdd
 *
 * @param data data return of the request
 * @param argc number of column
 * @param argv data of the column
 * @param azColName name of the column
 * @return int 0
 */
static int callbackIsNameInBdd(void* data, int argc, char** argv, char** azColName) {
    bool* nameExists = static_cast<bool*>(data);
    *nameExists = true;
    return 0;
}

/**
 * @brief Check if name is in bdd
 *
 * @param name name to check
 * @return true name is in bdd
 * @return false name is not in bdd
 */
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


/**
 * @brief Highscore object
 * 
 */
struct HighScore {
    /**
     * @brief name of the player
     * 
     */
    std::string name;
    /**
     * @brief score of the player
     * 
     */
    int score;
};

/**
 * @brief callback get high score
 *
 * @param data data return of the request
 * @param argc number of column
 * @param argv data of the column
 * @param azColName name of the column
 * @return int 0
 */
static int callbackGetHighScore(void* data, int argc, char** argv, char** azColName) {
    std::vector<HighScore>* results = static_cast<std::vector<HighScore>*>(data);
    results->push_back({argv[0], std::stoi(argv[1])});
    return 0;
}

/**
 * @brief Get the High Score on db
 *
 * @return HighScoreMessage high score message
 */
HighScoreMessage Server::getHighScore() {
    _logger.log("Getting best high scores");
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
    HighScoreMessage highscoreMsg(7, results[0].name, results[1].name, results[2].name, results[0].score, results[1].score, results[2].score, _packet_id);
    _packet_id += 1;
    _highscore_packets.push_back(highscoreMsg);
    return highscoreMsg;
}

/**
 * @brief Callback add get high score
 *
 * @param data data return of the request
 * @param argc number of column
 * @param argv data of the column
 * @param azColName name of the column
 * @return int 0
 */
void Server::addHighScore(std::string name, int score) {
    _logger.log("Adding high score from " + name + " : " + std::to_string(score));
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

/**
 * @brief Connect to database
 *
 */
void Server::connectToDB() {
    _logger.log("Connecting to database");
    int rc;
    rc = sqlite3_open("db/rtype.db", &_db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(_db) << std::endl;
        exit(84);
    } else {
        std::cerr << "Database opened successfully" << std::endl;
    }
}

/**
 * @brief make personnal ID
 *
 * @return std::string personnal ID
 */
std::string Server::makePersonnalID()
{
    std::string str("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");

    std::random_device rd;
    std::mt19937 generator(rd());

    std::shuffle(str.begin(), str.end(), generator);
    std::cout << str.substr(0, 10) << std::endl;
    return str.substr(0, 10);
}

/**
 * @brief Callback check if user exist
 *
 * @param data data return of the request
 * @param argc number of column
 * @param argv data of the column
 * @param azColName name of the column
 * @return int 0
 */
static int callbackIsUserExists(void* data, int argc, char** argv, char** azColName) {
    bool* nameExists = static_cast<bool*>(data);
    *nameExists = true;
    return 0;
}

/**
 * @brief Check if user exist
 *
 * @param name name of the user
 * @param password password of the user
 * @return true user exist
 * @return false user doesn't exist
 */
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

/**
 * @brief Callback check if user exist
 *
 * @param data data return of the request
 * @param argc number of column
 * @param argv data of the column
 * @param azColName name of the column
 * @return int 0
 */
bool Server::signUp(std::string name, std::string password) {
    _logger.log("User " + name + " is signing up");
    const std::string tableName = "USERS";
    if (checkIfUserExist(name, password) == true) {
        std::cout << "User already exist" << std::endl;
        return false;
    }
    std::string sql = "INSERT INTO " + tableName + " (id, name, password) VALUES ('" + makePersonnalID() +"', '" + name + "', '" + password + "');";
    char *zErrMsg = 0;
    int rc = sqlite3_exec(_db, sql.c_str(), NULL, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Records created successfully\n");
        return true;
    }
    return false;
}

/**
 * @brief Callback check if user exist
 *
 * @param data data return of the request
 * @param argc number of column
 * @param argv data of the column
 * @param azColName name of the column
 * @return int 0
 */
static int callbackSignIn(void *data, int argc, char** argv, char** azColName) {
    bool* userExists = static_cast<bool*>(data);
    *userExists = true;
    return 0;
}

/**
 * @brief Signin the user
 *
 * @param name name of the user
 * @param password password of the user
 * @return true user signed in
 * @return false user does not signed in
 */
bool Server::signIn(std::string name, std::string password) {
    _logger.log("User " + name + " is signing in");
    std::cout << "username :" << name << std::endl;
    std::cout << "password :" << password << std::endl;
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
    return userExists;
}

/**
 * @brief Callback check friendship
 *
 * @param data data return of the request
 * @param argc number of column
 * @param argv data of the column
 * @param azColName name of the column
 * @return int 0
 */
static int callbackCheckFriendship(void *data, int argc, char** argv, char** azColName) {
    bool* friendshipExists = static_cast<bool*>(data);
    *friendshipExists = true;
    return 0;
}

/**
 * @brief Check if friendship exist
 *
 * @param name name of the user
 * @param friendId name of the friend
 * @return true friendship exist
 * @return false friendship does not exist
 */
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

/**
 * @brief add friends
 *
 * @param name your name
 * @param friendId name of the friend
 * @return true if success
 * @return false if failed
 */
bool Server::addFriend(std::string name, std::string friendId) {
    _logger.log("User " + name + " is adding " + friendId + " as friend");
    if (checkIfFriendshipExist(name, friendId) == true) {
        std::cout << "Friendship already exist" << std::endl;
        return false;
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
    return true;
}

/**
 * @brief Remove friends
 *
 * @param name your name
 * @param friendId name of the friend
 * @return true if success
 * @return false if failed
 */
bool Server::removeFriend(std::string name, std::string friendName) {
    _logger.log("User " + name + " is removing " + friendName + " as friend");
    std::string sql = "DELETE FROM FRIENDS WHERE name = '" + name + "' AND friendId = '" + friendName + "';";
    char *zErrMsg = 0;
    int rc = sqlite3_exec(_db, sql.c_str(), NULL, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        return false;
    }
    return true;
}

/**
 * @brief Callback data structure
 * 
 */
struct CallbackData {
    /**
     * @brief pointer to the server object
     * 
     */
    Server* server;
    /**
     * @brief list of friends
     * 
     */
    std::vector<Friendship>* friends;
};

/**
 * @brief Callback display friends
 *
 * @param data data return of the request
 * @param argc number of column
 * @param argv data of the column
 * @param azColName name of the column
 * @return int 0
 */
static int callbackDisplayFriends(void *data, int argc, char** argv, char** azColName) {
    std::vector<std::string>* callbackData = static_cast<std::vector<std::string>*>(data);
    std::cout << argv[1] << std::endl;
    callbackData->push_back(std::string(argv[1]));
    return 0;
}

/**
 * @brief Display friends
 *
 * @param name name of the user
 * @return std::vector<std::string> list of friends
 */
std::vector<std::string> Server::displayFriends(std::string name, entity_t player_entity) {
    _logger.log("User " + name + " is displaying friends");
    const std::string tableName = "FRIENDS";
    std::string sql = "SELECT * FROM " + tableName + " WHERE name = '" + name + "';";
    char *zErrMsg = 0;
    std::vector<std::string> friends;
    int rc = sqlite3_exec(_db, sql.c_str(), callbackDisplayFriends, &friends, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    std::cout << friends.size() << std::endl;
    std::string str = "";
    std::string sep = "\n";
    for (const auto& friendData : friends) {
        std::cout << "Name: " << friendData << std::endl;
        str += friendData + sep;
    }
    std::cout << str << std::endl;
    FriendsResponse resp(9, str, _packet_id);
    send_data_to_client_by_entity<FriendsResponse>(resp, player_entity);
    return friends;
}
