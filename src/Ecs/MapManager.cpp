/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** MapManager
*/

#include "MapManager.hpp"

void MapManager::loadMaps()
{
    try {
        for (const auto& entry : std::filesystem::directory_iterator(_maps_folder))
            if (entry.path.filename().endswith(".txt"))
                this->loadMap(entry.path.filename());
            else
                std::cerr << "Skipping " << entry.path.filename() << ": not a valid map file." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error opening folder: " << _maps_folder << std::endl << e.what() << std::endl;
    }
}

void MapManager::loadMap(const std::string &filename)
{
    // load map 10x10
}
