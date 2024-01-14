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
    #include <unordered_map>
    #include <json.hpp>
    #include "FilesUtils.hpp"
    #include "../Ecs/Events.hpp"
    #include "../Ecs/Components.hpp"

using json = nlohmann::json;

struct properties {
    int health;
    int damage;
    float scale;
    component::Velocity velocity;
    properties(): velocity(component::Velocity(0.0f, 0.0f)) {};
    properties(int health_, int damage_, float scale_, std::pair<int, int> velocity_) : health(health_), damage(damage_), scale(scale_), velocity({static_cast<float>(velocity_.first), static_cast<float>(velocity_.second)}) {};
};

enum Entity_types {
    ANIMATED_ENTITY,
    STATIC_ENTITY,
    UNKNOWN_ENTITY
};

class ModelManager {
    public:
        ModelManager(EventListener &listener, registry &r) : _model_folder("assets/model/"), _listener(listener), _r(r) {};
        ~ModelManager() {};

        void loadModels(const std::string &room_name);

        std::unordered_map<std::string, component::AnimatedDrawable> getModelsForRoom(const std::string &room_name) const;
        animation_t getAnimsForEntity(const std::string &name) const;
        std::vector<std::string> getAliasesForEntity(const std::string &name) const;
        properties getPropertiesForEntity(const std::string &name) const;

    private:
        void loadModel(const std::string& filename, const std::string &room_name);
        std::string _model_folder;
        EventListener &_listener;
        registry &_r;
        std::unordered_map<std::string, std::unordered_map<std::string, component::AnimatedDrawable>> _entity_models;
        std::unordered_map<std::string, animation_t> _anims;
        std::unordered_map<std::string, std::vector<std::string>> _aliases;
        std::unordered_map<std::string, properties> _properties;
};

#endif /* !MODELMANAGER_HPP_ */
