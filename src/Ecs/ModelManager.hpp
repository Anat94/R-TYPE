/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ModelManager
*/

#ifndef MODELMANAGER_HPP_
    #define MODELMANAGER_HPP_
    #include <iostream>
    #include <filesystem>
    #include <map>
    #include <json.hpp>
    #include "FilesUtils.hpp"

using json = nlohmann::json;

enum Entity_types {
    ANIMATED_ENTITY,
    STATIC_ENTITY,
    UNKNOWN_ENTITY
};

class ModelManager {
    public:
        ModelManager() : _model_folder("assets/model/"), _models_types({
            {"animated_entity", ANIMATED_ENTITY},
            {"static_entity", STATIC_ENTITY}
        }) {};
        ~ModelManager() {};

        void loadModels();

    private:
        void loadModel(const std::string& filename);
        std::map<std::string, Entity_types> _models_types;
        std::string _model_folder;
};

#endif /* !MODELMANAGER_HPP_ */
