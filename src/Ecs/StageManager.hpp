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
    #include "FilesUtils.hpp"

using json = nlohmann::json;

class StageManager {
    public:
        StageManager() : _stages_folder("assets/stage/") {};
        ~StageManager() {};

        void loadStages();

    private:
        void loadStage(const std::string& filename);
        std::string _stages_folder;
};

#endif /* !STAGEMANAGER_HPP_ */
