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

// void Server::getHighScore() {
//     mongocxx::collection highScoreCollection = _rtypeDb["HighScore"];
//     auto cursor = highScoreCollection.find({});

//     for (auto&& doc : cursor) {
//         std::cout << bsoncxx::to_json(doc) << std::endl;
//         if (doc["value"].get_int32() > _highScore) {
//             _highScore = doc["value"].get_int32();
//             _nameForHighScore = doc["name"].get_utf8().value.to_string();
//         }
//     }
// }

// void Server::addHighScore(std::string name, int score) {
//     mongocxx::collection highScoreCollection = _rtypeDb["HighScore"];
//     bsoncxx::builder::stream::document document{};
//     document << "name" << name << "value" << score;
//     highScoreCollection.insert_one(document.view());
// }

// void Server::connectToDB() {
//     mongocxx::instance inst{};
//     const auto uri = mongocxx::uri{"mongodb+srv://anatolebabin:admin@cluster0.efwsldn.mongodb.net/?retryWrites=true&w=majority"};

//     mongocxx::options::client client_options;
//     const auto api = mongocxx::options::server_api{mongocxx::options::server_api::version::k_version_1};

//     client_options.server_api_opts(api);
//     _mongo_client = mongocxx::client{ uri, client_options };

//     _rtypeDb = _mongo_client["Rtype"];
//     const auto ping_cmd = bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("ping", 1));
//     _rtypeDb.run_command(ping_cmd.view());
//     std::cout << "Pinged your deployment. You successfully connected to MongoDB!" << std::endl;
// }

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
