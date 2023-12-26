/*
** EPITECH PROJECT, 2023
** R-TYPE
** File description:
** database.cpp
*/

#include "Server.hpp"
#include <random>
#include <sqlite3.h>
// #include <bsoncxx/builder/stream/document.hpp>
// #include <bsoncxx/json.hpp>

// std::vector<std::string> Server::getDatabases()
// {
// 	return _mongo_client.list_database_names();
// }


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
    // std::string sql12 = "DELETE FROM HIGHSCORE WHERE 1;";
    // char *zErrMsg = 0;
    // rc = sqlite3_exec(_db, sql12.c_str(), NULL, 0, &zErrMsg);
    // if (rc != SQLITE_OK) {
    //     fprintf(stderr, "SQL error: %s\n", zErrMsg);
    //     sqlite3_free(zErrMsg);
    // } else {
    //     fprintf(stdout, "Records created successfully\n");
    // }

}

// std::string Server::makePersonnalID()
// {
//     std::string str("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");

//     std::random_device rd;
//     std::mt19937 generator(rd());

//     std::shuffle(str.begin(), str.end(), generator);
//     std::cout << str.substr(0, 10) << std::endl;
//     return str.substr(0, 10);
// }

// void Server::signUp(std::string name, std::string password) {
//     mongocxx::collection playerCollection = _rtypeDb["Users"];
//     auto cursor = playerCollection.find({});
//     for (auto&& doc : cursor) {
//         if (doc["name"].get_utf8().value.to_string() == name) {
//             std::cout << "User already exists" << std::endl;
//             return;
//         }
//     }
//     bsoncxx::builder::stream::document document{};
//     document << "name" << name << "password" << password << "id" << makePersonnalID() << "friends" << bsoncxx::builder::stream::open_array << bsoncxx::builder::stream::close_array;
//     playerCollection.insert_one(document.view());
//     std::cout << "Connected" << std::endl;
// }

// void Server::signIn(std::string name, std::string password) {
//     mongocxx::collection playerCollection = _rtypeDb["Users"];
//     auto cursor = playerCollection.find({});

//     for (auto&& doc : cursor) {
//         if (doc["name"].get_utf8().value.to_string() == name && doc["password"].get_utf8().value.to_string() == password) {
//             std::cout << "Connected" << std::endl;
//             return;
//         }
//     }
//     std::cout << "Wrong password or username" << std::endl;
// }

// void Server::addFriend(std::string name, std::string friendId) {
//     mongocxx::collection playerCollection = _rtypeDb["Users"];
//     auto cursor = playerCollection.find({});

//     for (auto&& doc : cursor) {
//         if (doc["name"].get_utf8().value.to_string() == name) {
//             bsoncxx::builder::stream::document filter{};
//             filter << "name" << doc["name"].get_utf8().value.to_string();

//             bsoncxx::builder::stream::document updateDoc{};
//             updateDoc << "$push" << bsoncxx::builder::stream::open_document
//                       << "friends"
//                       << friendId
//                       << bsoncxx::builder::stream::close_document;

//             auto result = playerCollection.update_one(filter.view(), updateDoc.view());

//             if (result) {
//                 std::cout << "Friend added" << std::endl;
//             } else {
//                 std::cout << "Failed to update the document" << std::endl;
//             }

//             return;
//         }
//     }

//     std::cout << "User not found" << std::endl;
// }


// void Server::removeFriend(std::string name, std::string friendId) {
//     mongocxx::collection playerCollection = _rtypeDb["Users"];
//     auto cursor = playerCollection.find({});

//     for (auto&& doc : cursor) {
//         if (doc["name"].get_utf8().value.to_string() == name) {
//             bsoncxx::builder::stream::document filter{};
//             filter << "name" << doc["name"].get_utf8().value.to_string();

//             bsoncxx::builder::stream::document updateDoc{};
//             updateDoc << "$pull" << bsoncxx::builder::stream::open_document
//                       << "friends"
//                       << friendId
//                       << bsoncxx::builder::stream::close_document;

//             auto result = playerCollection.update_one(filter.view(), updateDoc.view());

//             if (result) {
//                 std::cout << "Friend removed" << std::endl;
//             } else {
//                 std::cout << "Failed to update the document" << std::endl;
//             }

//             return;
//         }
//     }

//     std::cout << "User not found" << std::endl;
// }

// using bsoncxx::builder::stream::document;
// void Server::displayFriends(std::string name) {
//     mongocxx::collection playerCollection = _rtypeDb["Users"];
//     auto cursor = playerCollection.find({});

//     for (auto&& doc : cursor) {
//         if (doc["name"].get_utf8().value.to_string() == name) {
//             std::cout << "Friends: " << std::endl;
//             printf("coucou je suis la");
//             for (auto&& friendId : doc["friends"].get_array().value) {
//                 printf("coucou je suis la");
//                 bsoncxx::builder::stream::document filter{};
//                 filter << "friendId" << friendId.get_utf8().value.to_string(); // Assuming friendId is the correct field name

//                 auto innerCursor = playerCollection.find(filter.view());

//                 for (auto&& innerDoc : innerCursor) {
//                     auto friendArray = innerDoc["friends"].get_array().value; // Assuming "friends" is the correct field name
//                     for (auto&& friendValue : friendArray) {
//                         std::string friendName = friendValue.get_utf8().value.to_string();
//                         std::cout << friendName << std::endl;
//                     }
//                 }
//             }
//             return;
//         }
//     }

//     std::cout << "User not found" << std::endl;
// }
