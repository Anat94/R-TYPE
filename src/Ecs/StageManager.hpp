/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** StageManager
*/

#ifndef STAGEMANAGER_HPP_
    #define STAGEMANAGER_HPP_
    #include <iostream>
    #include <filesystem>
    #include <json.hpp>
    #include <unordered_map>
    #include "FilesUtils.hpp"
    #include "../Ecs/Events.hpp"
    #include "../Ecs/Components.hpp"

using json = nlohmann::json;

class StageManager {
    public:
        StageManager(EventListener &listener, registry &r) : _stages_folder("assets/stage/"), _listener(listener), _r(r) {};
        ~StageManager() {};

        void loadStages(const std::string &room_name);

        std::unordered_map<std::string, std::vector<component::Drawable>> getBackgroundsForRoom(const std::string &room_name) const;
        std::unordered_map<std::string, std::vector<std::string>> getEnemiesForRoom(const std::string &room_name) const;
        std::vector<std::string> getStageNamesForRoom(const std::string &room_name) const;

    private:
        void loadStage(const std::string& filename, const std::string &room_name);
        std::string _stages_folder;
        EventListener &_listener;
        registry &_r;
        std::unordered_map<std::string, std::unordered_map<std::string, std::vector<component::Drawable>>> _backgrounds;
        std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::string>>> _enemies;
        std::unordered_map<std::string, std::vector<std::string>> _stage_names;
};

#endif /* !STAGEMANAGER_HPP_ */
