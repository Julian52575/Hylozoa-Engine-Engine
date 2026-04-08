/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Scene Serializer [source file]
*/

#include <fstream>
#include <iostream>
#include <unordered_map>

#include "Hylozoa-Engine/Components/Components.hpp"
#include "Hylozoa-Engine/Components/Scene/UUID.hpp"
#include "Hylozoa-Engine/Core/Entities/Entity.hpp"
#include "Hylozoa-Engine/Core/Settings.hpp"
#include "Scene.hpp"
#include "SceneSerializer.hpp"

namespace Hylozoa {

// --------------- Serialization Helpers ---------------

void SceneSerializer::serializeComponents(entt::entity entity,
                                          json &entityJson) {
    serializeIfPresent<Components::Name>(m_registry, entity,
                                         entityJson["Components"], "Name");
    serializeIfPresent<Components::LocalTransform>(
        m_registry, entity, entityJson["Components"], "LocalTransform");
    serializeIfPresent<Components::WorldTransform>(
        m_registry, entity, entityJson["Components"], "WorldTransform");
    serializeIfPresent<Components::Camera>(m_registry, entity,
                                           entityJson["Components"], "Camera");
    serializeIfPresent<Components::RigidBodyComponent>(
        m_registry, entity, entityJson["Components"], "RigidBodyComponent");
    serializeIfPresent<Components::ColliderComponent>(
        m_registry, entity, entityJson["Components"], "ColliderComponent");
    serializeIfPresent<Components::BoxColliderComponent>(
        m_registry, entity, entityJson["Components"], "BoxColliderComponent");
    serializeIfPresent<Components::CircleColliderComponent>(
        m_registry, entity, entityJson["Components"],
        "CircleColliderComponent");
    serializeIfPresent<Components::CapsuleColliderComponent>(
        m_registry, entity, entityJson["Components"],
        "CapsuleColliderComponent");
    serializeIfPresent<Components::Rendering::Renderable>(
        m_registry, entity, entityJson["Components"], "Renderable");
    serializeIfPresent<Components::Rendering::RenderableShape>(
        m_registry, entity, entityJson["Components"], "RenderableShape");
    serializeIfPresent<Components::Rendering::Sprite>(
        m_registry, entity, entityJson["Components"], "Sprite");
}

json SceneSerializer::serializeEntity(entt::entity entity) {
    json entityJson;

    entityJson["UUID"] =
        size_t{m_registry.get<Components::HylozoaInternal::Id>(entity).id};
    entityJson["Components"] = json::object();

    serializeComponents(entity, entityJson);
    serializeRelationships(entity, entityJson);

    return entityJson;
}

void SceneSerializer::serializeRelationships(entt::entity entity,
                                             json &entityJson) {
    if (m_registry.all_of<Components::HylozoaInternal::Parent>(entity)) {
        auto &parent =
            m_registry.get<Components::HylozoaInternal::Parent>(entity);

        if (m_registry.valid(parent.entity)) {
            UUID parentUUID =
                m_registry.get<Components::HylozoaInternal::Id>(parent.entity)
                    .id;
            entityJson["Parent"] = size_t(parentUUID);
        }
    }
}

void SceneSerializer::serializeScene(UUID sceneID, const std::string &path) {
    // Implementation for serializing the scene to a file at 'path'
    if (Hylozoa::Settings::getInstance().getSettings().verbose) {
        std::cout << "Serializing scene with ID " << sceneID << " to " << path
                  << std::endl;
    }

    auto view = m_registry.view<Components::HylozoaInternal::SceneTag>();
    json sceneJson;
    sceneJson["sceneID"] = size_t(sceneID);
    sceneJson["sceneName"] = "scene name";
    sceneJson["Entities"] = json::array();

    for (auto entity : view) {
        auto &sceneTag =
            view.get<Components::HylozoaInternal::SceneTag>(entity);

        if (sceneTag.id != sceneID)
            continue;
        sceneJson["Entities"].push_back(serializeEntity(entity));
    }

    writeToFile(sceneJson, path);
}

void SceneSerializer::serializeSceneRuntime(UUID sceneID,
                                            const std::string &path) {}

// --------------- Deserialization Helpers ---------------

void SceneSerializer::createEntities(
    const json &sceneJson, std::unordered_map<UUID, entt::entity> &entityMap) {
    for (const auto &entityJson : sceneJson["Entities"]) {
        UUID uuid(entityJson["UUID"].get<UUID>());

        Entity newEntity = m_sceneManager.spawnEntityFromUUIDInScene(
            uuid, UUID(sceneJson["sceneID"].get<UUID>()));
        entityMap[uuid] = newEntity.getHandle();
    }
}

void SceneSerializer::deserializeComponents(
    const json &sceneJson,
    const std::unordered_map<UUID, entt::entity> &entityMap) {
    for (const auto &entityJson : sceneJson["Entities"]) {
        UUID uuid(entityJson["UUID"].get<UUID>());
        entt::entity entity = entityMap.at(uuid);

        const json &components = entityJson["Components"];

        deserializeIfPresent<Components::Name>(m_registry, entity, components,
                                               "Name");

        deserializeIfPresent<Components::LocalTransform>(
            m_registry, entity, components, "LocalTransform");

        deserializeIfPresent<Components::WorldTransform>(
            m_registry, entity, components, "WorldTransform");

        deserializeIfPresent<Components::Camera>(m_registry, entity, components,
                                                 "Camera");
        deserializeIfPresent<Components::RigidBodyComponent>(
            m_registry, entity, components, "RigidBodyComponent");
        deserializeIfPresent<Components::ColliderComponent>(
            m_registry, entity, components, "ColliderComponent");
        deserializeIfPresent<Components::BoxColliderComponent>(
            m_registry, entity, components, "BoxColliderComponent");
        deserializeIfPresent<Components::CircleColliderComponent>(
            m_registry, entity, components, "CircleColliderComponent");
        deserializeIfPresent<Components::CapsuleColliderComponent>(
            m_registry, entity, components, "CapsuleColliderComponent");
        deserializeIfPresent<Components::Rendering::Renderable>(
            m_registry, entity, components, "Renderable");
        deserializeIfPresent<Components::Rendering::RenderableShape>(
            m_registry, entity, components, "RenderableShape");
        deserializeIfPresent<Components::Rendering::Sprite>(
            m_registry, entity, components, "Sprite");
    }
}

void SceneSerializer::deserializeRelationships(
    const json &sceneJson,
    const std::unordered_map<UUID, entt::entity> &entityMap) {
    for (const auto &entityJson : sceneJson["Entities"]) {
        if (!entityJson.contains("Parent"))
            continue;

        if (entityJson["Parent"].is_null())
            continue;
        UUID childUUID(entityJson["UUID"].get<UUID>());
        UUID parentUUID(entityJson["Parent"].get<UUID>());

        entt::entity child = entityMap.at(childUUID);
        entt::entity parent = entityMap.at(parentUUID);

        Entity childEntity = Entity::fromHandle(child, m_registry);

        childEntity.childOf(parent);
    }
}

void SceneSerializer::deserializeTextures(
    const json &sceneJson,
    const std::unordered_map<UUID, entt::entity> &entityMap) {
    auto spriteView = m_registry.view<Components::Rendering::Sprite>();

    for (auto entity : spriteView) {
        auto &sprite = spriteView.get<Components::Rendering::Sprite>(entity);
        Entity spriteEntity = Entity::fromHandle(entity, m_registry);

        spriteEntity.addComponent<Components::HylozoaInternal::RenderTexture>();
    }
}

UUID SceneSerializer::deserializeScene(const std::string &path) {
    json sceneJson;
    if (!readFromFile(path, sceneJson)) {
        std::cerr << "Failed to read scene file: " << path << std::endl;
        throw std::runtime_error(
            "SceneSerializer::deserializeScene - Failed to read scene file");
    }

    if (!sceneJson.contains("sceneID") || !sceneJson.contains("Entities")) {
        std::cerr << "Invalid scene file format: " << path << std::endl;
        throw std::runtime_error(
            "SceneSerializer::deserializeScene - Invalid scene file format");
    }

    UUID sceneId = m_sceneManager.createSceneWithUUID(
        sceneJson.value("sceneName", "UnnamedScene"),
        UUID(sceneJson["sceneID"].get<UUID>()));

    if (Hylozoa::Settings::getInstance().getSettings().verbose) {
        std::cout << "Deserializing scene with ID: " << sceneId << " from "
                  << path << std::endl;
    }

    std::unordered_map<UUID, entt::entity> relationMap;

    createEntities(sceneJson, relationMap);
    deserializeComponents(sceneJson, relationMap);
    deserializeRelationships(sceneJson, relationMap);
    deserializeTextures(sceneJson, relationMap);
    return sceneId;
}

UUID SceneSerializer::deserializeScene(const nlohmann::json &sceneJson) {

    if (!sceneJson.contains("sceneID") || !sceneJson.contains("Entities")) {
        std::cerr << "Invalid scene json format" << std::endl;
        throw std::runtime_error("SceneSerializer::deserializeScene (raw "
                                 "json)- Invalid scene json format");
    }

    UUID sceneId = m_sceneManager.createSceneWithUUID(
        sceneJson.value("sceneName", "UnnamedScene"),
        UUID(sceneJson["sceneID"].get<UUID>()));

    if (Hylozoa::Settings::getInstance().getSettings().verbose) {
        std::cout << "Deserializing scene with ID: " << sceneId << std::endl;
    }

    std::unordered_map<UUID, entt::entity> relationMap;

    createEntities(sceneJson, relationMap);
    deserializeComponents(sceneJson, relationMap);
    deserializeRelationships(sceneJson, relationMap);
    deserializeTextures(sceneJson, relationMap);
    return sceneId;
}

void SceneSerializer::deserializeSceneRuntime(uint64_t sceneID,
                                              const std::string &path) {}

// --------------- File I/O Helpers ---------------

bool SceneSerializer::writeToFile(const json &data, const std::string &path) {
    std::ofstream file(path);

    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing: " << path << std::endl;
        return false;
    }

    file << data.dump(4);
    return true;
}

bool SceneSerializer::readFromFile(const std::string &path, json &outJson) {
    std::ifstream file(path);

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << path << std::endl;
        return false;
    }

    try {
        file >> outJson;
    } catch (const json::parse_error &e) {
        std::cerr << "JSON parse error in file " << path << ": " << e.what()
                  << std::endl;
        return false;
    }

    return true;
}

} // namespace Hylozoa