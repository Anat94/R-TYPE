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
    #include <unordered_map>
    #include "FilesUtils.hpp"
    #include "../Ecs/Events.hpp"
    #include "../Ecs/Components.hpp"

using json = nlohmann::json;

class MapManager {
    public:
        MapManager(EventListener &listener, registry &r) : _maps_folder("assets/map/"), _r(r), _listener(listener) {};
        ~MapManager() {};

        void loadMaps(const std::string &room_name);

        std::unordered_map<std::string, std::vector<component::Position>> getEnemiesPositionForRoom(const std::string &room_name) const;

    private:
        void loadMap(const std::string& filename, const std::string &room_name);
        std::string _maps_folder;
        EventListener &_listener;
        registry &_r;
        std::unordered_map<std::string, std::unordered_map<std::string, std::vector<component::Position>>> _enemiesPositions;
};

#endif /* !MAPMANAGER_HPP_ */
