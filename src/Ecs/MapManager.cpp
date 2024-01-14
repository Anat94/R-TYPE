/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** MapManager
*/

#include "MapManager.hpp"

void MapManager::loadMaps(const std::string &room_name)
{
    try {
        for (const auto& entry : std::filesystem::directory_iterator(_maps_folder)) {
            std::string filename = entry.path().filename().string();
            if (filename.size() >= 4 && filename.substr(filename.size() - 4) == ".txt")
                this->loadMap(std::string(_maps_folder + filename), room_name);
            else
                std::cerr << "Skipping " << filename << ": not a valid map file." << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error opening folder: " << _maps_folder << std::endl << e.what() << std::endl;
    }
}

void MapManager::loadMap(const std::string &filename, const std::string &room_name)
{
    std::string mapContent = FilesUtils::readJsonFile(filename);

    std::unordered_map<std::string, std::vector<component::Position>> enemiesInRoom;

    int row = 0;
    int col = 0;
    const int cellSize = 100;

    for (char &cell : mapContent) {
        if (cell != ' ' && cell != '\n' && cell != '-') {
            std::string enemy_name = std::string(1, cell);
            component::Position pos = {static_cast<float>((1920 * static_cast<int>(row / 10)) + col * cellSize), static_cast<float>((row % 10) * cellSize)};
            enemiesInRoom[enemy_name].push_back(pos);
        }
        ++col;
        if (cell == '\n') {
            ++row;
            col = 0;
        }
    }

    _enemiesPositions[room_name] = enemiesInRoom;
}

std::unordered_map<std::string, std::vector<component::Position>> MapManager::getEnemiesPositionForRoom(const std::string &room_name) const
{
    auto it = _enemiesPositions.find(room_name);

    if (it != _enemiesPositions.end())
        return it->second;
    else
        throw std::runtime_error("No positions for the asked room");
}
