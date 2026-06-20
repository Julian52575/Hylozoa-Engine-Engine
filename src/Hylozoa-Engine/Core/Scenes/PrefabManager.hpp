/*
** EPITECH PROJECT, 2026
** Hylozoa Engine
** File description:
** Prefab Manager [header file]
*/

#pragma once

#include <unordered_map>
#include "Hylozoa-Engine/Core/Entities/Entity.hpp"
#include "Hylozoa-Engine/Core/jsonWrap.hpp"


namespace Hylozoa {

class SceneSerializer;

class PrefabManager {
public:
    PrefabManager(const PrefabManager &) = delete;
    PrefabManager &operator=(const PrefabManager &) = delete;

    static PrefabManager &instance() {
        static PrefabManager instance;
        return instance;
    }

/**
 * @brief Preloads a prefab from a file at the specified path and stores it in memory for later instantiation.
 * 
     * this method is use to preload a prefab from a json file for later use,
     * it will read the prefab file and store its json content in a map with the prefab name as key,
     * so that it can be instantiated later without having to read the file again.
     * 
     * @param prefabPath path to the prefab file, relative to the Assets/ directory.
     */
    bool loadPrefab(const std::string& prefabPath);
    /**
     * @brief Preloads a prefab from raw json content and stores it in memory for later instantiation.
     * 
     * this method is use to preload a prefab from raw json content for later use,
     * it will read the prefab file and store its json content in a map with the prefab name as key,
     * so that it can be instantiated later.
     * 
     * @param prefabPath path to the prefab file, relative to the Assets/ directory.
     */
    bool loadPrefab(json& prefabJson);
    
    /**
     * @brief Instantiates a prefab at a given position in the scene.
     * 
     * The prefab should have been preloaded using loadPrefab() before calling this method.
     * The method will create a new entity in the currently active scene with the same components and properties as defined in the prefab, and set its position to the specified value.
     * 
     * @param prefab name of the prefab to instantiate (should match the name used in loaded prefab file/content).
     * @param position position to instantiate the prefab at, in world coordinates.
     * @param registry entt registry to use for creating the new entity.
     * @param sceneManager reference to the SceneManager to use for spawning the entity in the current scene.
     * @return Entity an Entity wrapper of the new entity created from the prefab in the currently active scene.
     */
    Entity instantiatePrefab(const std::string& prefab, const glm::vec2& position, SceneSerializer& sceneManager);
    
private:
    PrefabManager() = default;
    std::unordered_map<std::string, json> m_loadedPrefabs;
};

}