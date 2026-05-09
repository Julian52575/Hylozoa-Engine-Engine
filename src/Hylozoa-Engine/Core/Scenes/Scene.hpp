/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Scene Core [header file]
*/

#pragma once

#include "../Entities/Entity.hpp"
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

/**
 * @class Scene
 * @brief Represents a scene in the engine.
 *
 * Scenes do not directly own entities, is more of a logical grouping of
 * entities by ids.
 */
class Scene {
  public:
    Scene(UUID scene_id, const std::string &scene_name)
        : m_id(scene_id), m_name(scene_name) {};
    ~Scene() = default;

    /**
     * @brief returns the UUID of the scene.
     *
     * @return UUID
     */
    UUID id() const { return m_id; }
    /**
     * @brief returns the name of the scene.
     *
     * @return std::string
     */
    std::string name() const { return m_name; }

    /**
     * @brief Spawns a new entity in the scene with the given name and adds it
     * to the registry.
     *
     * The new entity is atomatically assigned a UUID and a SceneTag component
     * that associates it with the scene. The entity is also given a Name
     * component with the provided name and a default LocalTransform component.
     * The method returns the newly spawned Entity.
     *
     * @param name The name to assign to the spawned entity.
     * @param registry The entt registry to which the new entity will be added.
     * @return Entity the newly spawned entity in the scene.
     */
    Entity spawnEntityInScene(std::string &name, entt::registry &registry);
    /**
     * @brief Spawns a new raw entity in the scene and adds it to the registry.
     *
     * The new entity is automatically assigned a SceneTag component that
     * associates it with the scene and a SceneActiveTag to mark it as active.
     *
     * @param registry the entt registry to which the new entity will be added.
     * @return Entity the newly spawned raw entity in the scene.
     */
    Entity spawnRawEntity(entt::registry &registry);
    /**
     * @brief Spawns a new entity in the scene with the given UUID and adds it
     * to the registry.
     *
     * The new entity is automatically assigned a SceneTag component that
     * associates it with the scene, an UUID and a SceneActiveTag to mark it as
     * active.
     *
     * @param uuid the UUID to assign to the spawned entity.
     * @param registry the entt registry to which the new entity will be added.
     * @return Entity the newly spawned entity in the scene.
     */
    Entity spawnEntityFromUUID(UUID uuid, entt::registry &registry);

    /**
     * @brief Destroys the scene
     *
     * This method is responsible for destroying the scene and all entities
     * associated with it. It removes all entities that have a SceneTag
     * component matching the scene's UUID from the registry, effectively
     * cleaning up all resources associated with the scene. After calling this
     * method, the scene should be considered destroyed and should not be used
     * anymore.
     *
     * @param registry
     */
    void destroyScene(entt::registry &registry);

  private:
    UUID m_id;
    std::string m_name;
};

/**
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

    /**
     * @brief Initializes the SceneManager by creating and loading a default
     * scene.
     */
    void initialize();

    /**
     * @brief Creates a new scene with the given name.
     */
    UUID createScene(const std::string &name);

    /**
     * @brief Creates a new scene with the given name and specified UUID.
     */
    UUID createSceneWithUUID(const std::string &name, UUID uuid);

    /**
     * @brief Destroys a scene by its name.
     */
    void destroyScene(const std::string &name);

    /**
     * @brief Destroys a scene by its UUID.
     *
     */
    void destroyScene(const UUID id);

    /**
     * @brief clears all scenes from the engine.
     *
     * Unloading them if they are currently loaded and removing them from the
     * scene management system. After calling this method, there will be no
     * scenes registered in the engine. This is used primarily for cleanup
     * purposes, such as when Stopping the engine or completely shutting it
     * down.
     */
    void clearScenes();

    /**
     * @brief Loads a scene by name.
     */
    void loadScene(const std::string &name);
    /**
     * @brief Loads a scene by ID.
     */
    void loadScene(const UUID id);

    /**
     * @brief Unloads a scene by name.
     */
    void unloadScene(const std::string &name);
    /**
     * @brief Unloads a scene by ID.
     */
    void unloadScene(const UUID id);

    /**
     * @brief Spawns a new entity in the currently active scene.
     * @param name The name of the entity to spawn (optional).
     * @returns The spawned Entity.
     * @throws std::runtime_error if no scene is loaded.
     *
     * Note: The entity is spawned in the most recently loaded scene.
     */
    Entity spawnEntity(std::string name = std::string());

    /**
     * @brief Spawns a new entity in the specified scene.
     * @param name The name of the entity to spawn (optional).
     * @param sceneID The ID of the scene to spawn the entity in.
     * @returns The spawned Entity.
     * @throws std::runtime_error if the specified scene does not exist.
     */
    Entity spawnEntityInScene(std::string name, UUID sceneID);

    /**
     * @brief Spawns a new entity with the specified UUID in the specified
     * scene.
     * @param uuid The UUID of the entity to spawn.
     * @param sceneID The ID of the scene to spawn the entity in.
     * @returns The spawned Entity.
     * @throws std::runtime_error if the specified scene does not exist.
     */
    Entity spawnEntityFromUUIDInScene(UUID uuid, UUID sceneID);

    /**
     * @brief Get the SceneSerializer instance for this SceneManager.
     *
     * @return SceneSerializer&
     */
    SceneSerializer &serializer() { return m_sceneSerializer; }

  private:
    std::unordered_map<UUID, std::unique_ptr<Scene>> m_scenesById;

    std::vector<UUID> m_loadedScenes;

    entt::registry &m_registry;

    SceneSerializer m_sceneSerializer{m_registry, *this};

  private:
    void activateScene(UUID id);
    void deactivateScene(UUID id);
};

} // namespace Hylozoa