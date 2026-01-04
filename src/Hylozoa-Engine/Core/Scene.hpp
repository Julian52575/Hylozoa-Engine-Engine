/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Scene Core [header file]
*/

#pragma once

#include "Entity.hpp"

#include <entt/entt.hpp>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <memory>
#include <vector>

namespace Hylozoa {

/*
* @class Scene
* @brief Represents a scene in the engine.
*
* Scenes do not directly own entities, is more of a logical grouping of entities by ids.
*/
class Scene {
public:
  Scene(uint64_t scene_id, std::string scene_name)
      : m_id(scene_id), m_name(scene_name) {};
  ~Scene() = default;

  uint64_t id() const { return m_id; }
  std::string name() const { return m_name; }

  Entity spawnEntityInScene(std::string& name, entt::registry& registry);

  private:
    uint64_t m_id{0};
    std::string m_name{""};
};

/*
* @class SceneManager
* 
* @brief Manages scenes within the engine.
*
* This class handles the creation, loading, unloading, and management of scenes.
* It allows for spawning entities within the currently active scene.
*/
class SceneManager {
public:
  SceneManager(entt::registry& registry);
  ~SceneManager() = default;

  /*
  * @brief Initializes the SceneManager by creating and loading a default scene.
  */
  void initialize();

  /*
  * @brief Creates a new scene with the given name.
  */
  uint64_t createScene(const std::string& name);

  /*
  * @brief Loads a scene by name.
  */
  void loadScene(const std::string& name);
  /*
  * @brief Loads a scene by ID.
  */
  void loadScene(const uint64_t id);
  
  /*
  * @brief Unloads a scene by name.
  */
  void unloadScene(const std::string& name);
  /*
  * @brief Unloads a scene by ID.
  */
  void unloadScene(const uint64_t id);
  
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
  Entity spawnEntityInScene(std::string name = std::string(), uint64_t sceneID = 0);
private:
  std::unordered_map<uint64_t, std::unique_ptr<Scene>> m_scenesById;
  
  std::vector<uint64_t> m_loadedScenes;
  
  entt::registry& m_registry;

  void activateScene(const uint64_t id);
  void deactivateScene(const uint64_t id);
};

/*
* @class SceneSerializer
* @brief Handles serialization and deserialization of scenes.
*
* This class provides methods to serialize and deserialize scenes to and from files.
*/
class SceneSerializer {
public:
  SceneSerializer(entt::registry &registry) : m_registry(registry) {};
  ~SceneSerializer() = default;

  void serializeScene(uint64_t sceneID, const std::string &path);
  void serializeSceneRuntime(uint64_t sceneID, const std::string &path);
  void deserializeScene(uint64_t sceneID, const std::string &path);
  void deserializeSceneRuntime(uint64_t sceneID, const std::string &path);
private:

  entt::registry& m_registry;
};

} // namespace Hylozoa