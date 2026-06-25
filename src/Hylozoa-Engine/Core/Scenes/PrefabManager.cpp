/*
** EPITECH PROJECT, 2026
** Hylozoa Engine
** File description:
** Prefab Manager [source file]
*/

#include "Hylozoa-Engine/Core/Settings.hpp"
#include "PrefabManager.hpp"
#include "SceneSerializer.hpp"

namespace Hylozoa {

bool PrefabManager::loadPrefab(const std::string& prefabPath) {
    json prefabJson;

    if (!SceneSerializer::readFromFile(prefabPath, prefabJson)) {
        std::cerr << "PrefabManager::loadPrefab - Failed to load prefab from " << prefabPath << std::endl;
        return false;
    }

    normalizeKeys(prefabJson);
    if (!prefabJson.contains("prefabname")) {
        std::cerr << "PrefabManager::loadPrefab - Prefab JSON does not contain 'prefabName' field: " << prefabPath << std::endl;
        return false;
    }
    if (!prefabJson["prefabname"].is_string()) {
        std::cerr << "PrefabManager::loadPrefab - Prefab JSON field 'prefabName' must be a string: " << prefabPath << std::endl;
        return false;
    }

    std::string prefabName = prefabJson["prefabname"].get<std::string>();
    m_loadedPrefabs[prefabName] = prefabJson;
    return true;
}

bool PrefabManager::loadPrefab(json& prefabJson) {
    normalizeKeys(prefabJson);
    if (!prefabJson.contains("prefabname")) {
        std::cerr << "PrefabManager::loadPrefab (raw json) - Prefab JSON does not contain 'prefabName' field." << std::endl;
        return false;
    }
    if (!prefabJson["prefabname"].is_string()) {
        std::cerr << "PrefabManager::loadPrefab (raw json) - Prefab JSON field 'prefabName' must be a string." << std::endl;
        return false;
    }

    std::string prefabName = prefabJson["prefabname"].get<std::string>();
    m_loadedPrefabs[prefabName] = prefabJson;
    return true;
}

Entity PrefabManager::instantiatePrefab(const std::string& prefab, const glm::vec2& position, SceneSerializer& sceneSerializer) {
    auto it = m_loadedPrefabs.find(prefab);
    if (it == m_loadedPrefabs.end()) {
        if (Hylozoa::Settings::getInstance().getSettings().verbose) {
            std::cerr << "PrefabManager::instantiatePrefab - Prefab '" << prefab << "' not found. Make sure it is loaded before instantiating." << std::endl;
        }
        throw std::runtime_error("PrefabManager::instantiatePrefab - Prefab not found");
    }

    json& prefabJson = it->second;
    return sceneSerializer.deserializePrefab(prefabJson, position);
}

}
