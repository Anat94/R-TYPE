/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** StageManager
*/

#include "StageManager.hpp"

void StageManager::loadStages(const std::string &room_name)
{
    try {
        for (const auto& entry : std::filesystem::directory_iterator(_stages_folder)) {
            std::string filename = entry.path().filename().string();
            if (filename.size() >= 5 && filename.substr(filename.size() - 5) == ".json")
                this->loadStage(std::string(_stages_folder + filename), room_name);
            else
                std::cerr << "Skipping " << filename << ": not a valid json file." << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error opening folder: " << _stages_folder << std::endl << e.what() << std::endl;
    }
}

void StageManager::loadStage(const std::string &filename, const std::string &room_name)
{
    std::string jsonContent = FilesUtils::readJsonFile(filename);

    if (jsonContent.empty()) {
        std::cerr << "Skipping " << filename << ": not a valid json file." << std::endl;
        return;
    }

    json j = nlohmann::json::parse(jsonContent);

    try {
        // checks for required fields

        std::string name = j.at("name");
        std::string map_path = j.at("map");
        std::vector<std::string> textures = j.at("texture");
        std::vector<std::string> enemies = j.at("enemy");
        std::vector<std::string> win_conditions = j.at("win_condition");

        if (_stage_names.find(room_name) == _stage_names.end())
            _stage_names[room_name] = {};
        
        _stage_names[room_name].push_back(name);

        if (_backgrounds.find(room_name) == _backgrounds.end())
            _backgrounds[room_name] = {};
        
        std::vector<component::Drawable> backgrounds;

        for (auto texture : textures)
            backgrounds.push_back(component::Drawable(texture));
        
        _backgrounds[room_name][name] = backgrounds;

        if (_enemies.find(room_name) == _enemies.end())
            _enemies[room_name] = {};

        _enemies[room_name][name] = enemies;
    } catch (const std::exception& e) {
        std::cerr << "Skipping " << filename << ": not a valid json file." << std::endl;
        return;
    }
}

std::unordered_map<std::string, std::vector<component::Drawable>> StageManager::getBackgroundsForRoom(const std::string &room_name) const
{
    auto it = _backgrounds.find(room_name);

    if (it != _backgrounds.end())
        return it->second;
    else
        throw std::runtime_error("No background for the asked room");
}

std::unordered_map<std::string, std::vector<std::string>> StageManager::getEnemiesForRoom(const std::string &room_name) const
{
    auto it = _enemies.find(room_name);

    if (it != _enemies.end())
        return it->second;
    else
        throw std::runtime_error("No enemies found for the asked room");
}

std::vector<std::string> StageManager::getStageNamesForRoom(const std::string &room_name) const
{
    auto it = _stage_names.find(room_name);

    if (it != _stage_names.end())
        return it->second;
    else
        throw std::runtime_error("No names found for the asked room");
}
