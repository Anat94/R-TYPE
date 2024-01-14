/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** MapManager
*/

#ifndef MAPMANAGER_HPP_
    #define MAPMANAGER_HPP_
    #include <iostream>
    #include <filesystem>
    #include <json.hpp>
    #include "FilesUtils.hpp"

using json = nlohmann::json;

class MapManager {
    public:
        MapManager() : _maps_folder("assets/map/") {};
        ~MapManager() {};

        void loadMaps();

    private:
        void loadMap(const std::string& filename);
        std::string _maps_folder;
};

#endif /* !MAPMANAGER_HPP_ */
