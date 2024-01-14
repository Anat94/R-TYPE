/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** StageManager
*/

#include "StageManager.hpp"

void StageManager::loadStages()
{
    try {
        for (const auto& entry : std::filesystem::directory_iterator(_stages_folder))
            if (entry.path.filename().endswith(".json"))
                this->loadStage(entry.path.filename());
            else
                std::cerr << "Skipping " << entry.path.filename() << ": not a valid json file." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error opening folder: " << _stages_folder << std::endl << e.what() << std::endl;
    }
}

void StageManager::loadStage(const std::string &filename)
{
    std::string jsonContent = FilesUtils::readJsonFile(filename);

    if (jsonContent.empty()) {
        std::cerr << "Skipping " << filename << ": not a valid json file." << std::endl;
        return;
    }

    json j = nlohmann::json::parse(jsonContent);

    try {
        // checks for required fields
        j["name"];
        j["map"];
        j["enemies"];
        j["win_condition"];
        j["rewards"];
    } catch (const std::exception& e) {
        std::cerr << "Skipping " << filename << ": not a valid json file." << std::endl;
        return;
    }
}
