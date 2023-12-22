/*
** EPITECH PROJECT, 2023
** R-TYPE
** File description:
** database.cpp
*/

#include "Server.hpp"

std::vector<std::string> Server::getDatabases()
{
	return _mongo_client.list_database_names();
}

void Server::getHighScore() {
    mongocxx::database rtypeDb = _mongo_client["Rtype"];
    mongocxx::collection highScoreCollection = rtypeDb["HighScore"];
    auto cursor = highScoreCollection.find({});

    for (auto&& doc : cursor) {
        std::cout << bsoncxx::to_json(doc) << std::endl;
        if (doc["value"].get_int32() > _highScore) {
            _highScore = doc["value"].get_int32();
            _nameForHighScore = doc["name"].get_utf8().value.to_string();
        }
    }
}

void Server::addHighScore(std::string name, int score) {
    mongocxx::database rtypeDb = _mongo_client["Rtype"];
    mongocxx::collection highScoreCollection = rtypeDb["HighScore"];
    bsoncxx::builder::stream::document document{};
    document << "name" << name << "value" << score;
    highScoreCollection.insert_one(document.view());
}

void Server::connectToDB() {
    mongocxx::instance inst{};
    const auto uri = mongocxx::uri{"mongodb+srv://anatolebabin:admin@cluster0.efwsldn.mongodb.net/?retryWrites=true&w=majority"};

    mongocxx::options::client client_options;
    const auto api = mongocxx::options::server_api{mongocxx::options::server_api::version::k_version_1};

    client_options.server_api_opts(api);
    _mongo_client = mongocxx::client{ uri, client_options };

    mongocxx::database db = _mongo_client["admin"];
    const auto ping_cmd = bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("ping", 1));
    db.run_command(ping_cmd.view());
    std::cout << "Pinged your deployment. You successfully connected to MongoDB!" << std::endl;
}