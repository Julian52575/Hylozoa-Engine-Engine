/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Scene Serializer [header file]
*/

#pragma once

#include "Hylozoa-Engine/Components/Scene/UUID.hpp"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <entt/entt.hpp>

namespace Hylozoa {
class SceneManager;

/*
 * @class SceneSerializer
 * @brief Handles serialization and deserialization of scenes.
 *
 * This class provides methods to serialize and deserialize scenes to and from
 * files.
 */
class SceneSerializer {
  public:
    SceneSerializer(entt::registry &registry, SceneManager &manager)
        : m_registry(registry), m_sceneManager(manager) {};
    ~SceneSerializer() = default;

    /*
     * @brief Serializes the scene with the given ID to a file at the specified
     * path.
     *
     * @param sceneID The UUID of the scene to serialize.
     * @param path The file path to save the serialized scene.
     */
    void serializeScene(UUID sceneID, const std::string &path);
    void serializeSceneRuntime(UUID sceneID, const std::string &path);
    /*
     * @brief Deserializes a scene from a file at the specified path.
     * @param path The file path to load the scene from.
     * @returns The UUID of the deserialized scene.
     * @throws std::runtime_error if the file format is invalid.
     * @warning The scene will be created but NOT loaded/activated.
     */
    UUID deserializeScene(const std::string &path);
    void deserializeSceneRuntime(uint64_t sceneID, const std::string &path);

  private:
    bool writeToFile(const json &data, const std::string &path);
    bool readFromFile(const std::string &path, json &outJson);
    json serializeEntity(entt::entity entity);
    void serializeComponents(entt::entity entity, json &entityJson);
    void serializeRelationships(entt::entity entity, json &entityJson);
    void createEntities(const json &sceneJson,
                        std::unordered_map<UUID, entt::entity> &entityMap);
    void deserializeComponents(
        const json &sceneJson,
        const std::unordered_map<UUID, entt::entity> &entityMap);
    void deserializeRelationships(
        const json &sceneJson,
        const std::unordered_map<UUID, entt::entity> &entityMap);
    void deserializeTextures(
        const json &sceneJson,
        const std::unordered_map<UUID, entt::entity> &entityMap);
    entt::registry &m_registry;
    SceneManager &m_sceneManager;
};

} // namespace Hylozoa