/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ModelManager
*/

#include "ModelManager.hpp"

void ModelManager::loadModels(const std::string &room_name)
{
    try {
        for (const auto& entry : std::filesystem::directory_iterator(_model_folder)) {
            std::string filename = entry.path().filename().string();
            if (filename.size() >= 5 && filename.substr(filename.size() - 5) == ".json") {
                try {
                    this->loadModel(std::string(_model_folder + filename), room_name);
                } catch (const std::exception& e) {
                    std::cout << e.what() << std::endl;
                }
            } else
                std::cerr << "Skipping " << filename << ": not a valid json file." << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error opening folder: " << _model_folder << std::endl << e.what() << std::endl;
    }
}

void ModelManager::loadModel(const std::string &filename, const std::string &room_name)
{
    std::string jsonContent = FilesUtils::readJsonFile(filename);

    if (jsonContent.empty()) {
        std::cerr << "Skipping " << filename << ": not a valid json file." << std::endl;
        return;
    }

    std::cout << "Loading " << filename << std::endl;

    json j = nlohmann::json::parse(jsonContent);

    try {
        // checks for required fields

        std::string name = j.at("name");
        std::vector<std::string> aliases = j.at("aliases");
        std::string type = j.at("type");
        int health = j.at("health");
        int velocity = j.at("velocity");
        int damage = j.at("damage");
        float scale = j.at("scale");
        std::string texture = j.at("texture");
        int nb_sprites = j.at("number_of_sprites");
        std::vector<int> sprite_size =  j.at("sprite_size");
        std::vector<int> gaps = j.at("gaps");
        std::vector<int> offsets = j.at("offset");
        auto anims = j.at("animations");

        if (_entity_models.find(room_name) == _entity_models.end()) {
            _entity_models[room_name] = {};
        }

        _entity_models[room_name][name] = component::AnimatedDrawable(texture, {nb_sprites, 1}, {sprite_size[0], sprite_size[1]}, {gaps[0], gaps[1]}, {offsets[0], offsets[1]});
        if (_anims.find(name) == _anims.end()) {
            _anims[name] = {};
        }

        for (const auto& anim : j.at("animations")) {
            std::string animName = anim.at("name");
            bool reset = anim.at("reset");
            std::vector<int> indexes = anim.at("sprites");

            _anims[name].insert({animName, std::make_pair(reset, std::make_pair(indexes[0], indexes[1]))});
        }

        if (_aliases.find(name) == _aliases.end()) {}
            _aliases[name] = {};
        
        _aliases[name] = aliases;

        if (_properties.find(name) == _properties.end())
            _properties[name] = properties(health, damage, scale, {velocity, 0});
    } catch (const std::exception& e) {
        std::cerr << "Skipping " << filename << ": not a valid json file." << std::endl;
        return;
    }
}

std::unordered_map<std::string, component::AnimatedDrawable> ModelManager::getModelsForRoom(const std::string& room_name) const
{
    auto it = _entity_models.find(room_name);

    if (it != _entity_models.end()) {
        return it->second;
    } else
        throw std::runtime_error("No entities for the asked room");
}

animation_t ModelManager::getAnimsForEntity(const std::string &name) const
{
    auto it = _anims.find(name);

    if (it != _anims.end())
        return it->second;
    else
        throw std::runtime_error("No animations for the asked entity");
}

std::vector<std::string> ModelManager::getAliasesForEntity(const std::string &name) const
{
    auto it = _aliases.find(name);

    if (it != _aliases.end())
        return it->second;
    else
        throw std::runtime_error("No aliases for the asked entity");
}

properties ModelManager::getPropertiesForEntity(const std::string &name) const
{
    auto it = _properties.find(name);

    if (it != _properties.end())
        return it->second;
    else
        throw std::runtime_error("No properties for the asked entity");
}
