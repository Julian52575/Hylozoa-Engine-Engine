/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Scene Core [header file]
*/

#pragma once

#include "Entity.hpp"
#include "Hylozoa-Engine/Components/Components.hpp"
#include "Hylozoa-Engine/Components/Scene/UUID.hpp"
#include "SceneSerializer.hpp"

#include <cstdint>
#include <entt/entt.hpp>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace Hylozoa {

/*
 * @class Scene
 * @brief Represents a scene in the engine.
 *
 * Scenes do not directly own entities, is more of a logical grouping of
 * entities by ids.
 */
class Scene {
  public:
    Scene(UUID scene_id, const std::string &scene_name)
        : m_id(scene_id), m_name(scene_name){};
    ~Scene() = default;

    UUID id() const { return m_id; }
    std::string name() const { return m_name; }

    Entity spawnEntityInScene(std::string &name, entt::registry &registry);
    Entity spawnRawEntity(entt::registry &registry);
    Entity spawnEntityFromUUID(UUID uuid, entt::registry &registry);

  private:
    UUID m_id;
    std::string m_name;
};

/*
 * @class SceneManager
 *
 * @brief Manages scenes within the engine.
 *
 * This class handles the creation, loading, unloading, and management of
 * scenes. It allows for spawning entities within the currently active scene.
 */
class SceneManager {
  public:
    SceneManager(entt::registry &registry);
    ~SceneManager() = default;

    /*
     * @brief Initializes the SceneManager by creating and loading a default
     * scene.
     */
    void initialize();

    /*
     * @brief Creates a new scene with the given name.
     */
    UUID createScene(const std::string &name);

    /*
     * @brief Creates a new scene with the given name and specified UUID.
     */
    UUID createSceneWithUUID(const std::string &name, UUID uuid);

    /*
     * @brief Loads a scene by name.
     */
    void loadScene(const std::string &name);
    /*
     * @brief Loads a scene by ID.
     */
    void loadScene(const UUID id);

    /*
     * @brief Unloads a scene by name.
     */
    void unloadScene(const std::string &name);
    /*
     * @brief Unloads a scene by ID.
     */
    void unloadScene(const UUID id);

    /*
     * @brief Spawns a new entity in the currently active scene.
     * @param name The name of the entity to spawn (optional).
     * @returns The spawned Entity.
     * @throws std::runtime_error if no scene is loaded.
     *
     * Note: The entity is spawned in the most recently loaded scene.
     */
    Entity spawnEntity(std::string name = std::string());

    /*
     * @brief Spawns a new entity in the specified scene.
     * @param name The name of the entity to spawn (optional).
     * @param sceneID The ID of the scene to spawn the entity in.
     * @returns The spawned Entity.
     * @throws std::runtime_error if the specified scene does not exist.
     */
    Entity spawnEntityInScene(std::string name = std::string(),
                              UUID sceneID = UUID());

    /*
     * @brief Spawns a new entity with the specified UUID in the specified
     * scene.
     * @param uuid The UUID of the entity to spawn.
     * @param sceneID The ID of the scene to spawn the entity in.
     * @returns The spawned Entity.
     * @throws std::runtime_error if the specified scene does not exist.
     */
    Entity spawnEntityFromUUIDInScene(UUID uuid, UUID sceneID);

    SceneSerializer &serializer() { return m_sceneSerializer; }

  private:
    std::unordered_map<UUID, std::unique_ptr<Scene>> m_scenesById;

    std::vector<UUID> m_loadedScenes;

    entt::registry &m_registry;

    SceneSerializer m_sceneSerializer{m_registry, *this};

    void activateScene(UUID id);
    void deactivateScene(UUID id);
};

} // namespace Hylozoa