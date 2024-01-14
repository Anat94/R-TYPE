/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ModelManager
*/

#include "ModelManager.hpp"

void ModelManager::loadModels()
{
    try {
        for (const auto& entry : std::filesystem::directory_iterator(_model_folder))
            if (entry.path.filename().endswith(".json"))
                this->loadModel(entry.path.filename());
            else
                std::cerr << "Skipping " << entry.path.filename() << ": not a valid json file." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error opening folder: " << _model_folder << std::endl << e.what() << std::endl;
    }
}

void ModelManager::loadModel(const std::string &filename)
{
    std::string jsonContent = FilesUtils::readJsonFile(filename);

    if (jsonContent.empty()) {
        std::cerr << "Skipping " << filename << ": not a valid json file." << std::endl;
        return;
    }

    json j = nlohmann::json::parse(jsonContent);

    try {
        // checks for required fields
        j["name"];
        j["aliases"];
        j["type"];
        j["health"];
        j["damage"];
        j["scale"];
        j["texture"];
        j["number_of_sprites"];
        j["sprite_size"];
        j["gaps"];
        j["offset"];
        j["animations"];
    } catch (const std::exception& e) {
        std::cerr << "Skipping " << filename << ": not a valid json file." << std::endl;
        return;
    }
}
