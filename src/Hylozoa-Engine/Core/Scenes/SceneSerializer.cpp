/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Scene Serializer [source file]
*/

#include <fstream>
#include <iostream>
#include <unordered_map>

#include "SceneSerializer.hpp"
#include "Scene.hpp"
#include "Hylozoa-Engine/Components/Scene/UUID.hpp"
#include "Hylozoa-Engine/Components/Components.hpp"
#include "Hylozoa-Engine/Core/Entities/Entity.hpp"
#include "Hylozoa-Engine/Core/Script/ScriptManager.hpp"
#include "Hylozoa-Engine/Core/Settings.hpp"

namespace Hylozoa {

// --------------- Serialization Helpers ---------------

void SceneSerializer::serializeComponents(entt::entity entity,
                                          json &entityJson) {
    serializeIfPresent<Components::Name>(
    m_registry, entity, entityJson["Components"], "Name");
    serializeIfPresent<Components::LocalTransform>(
    m_registry, entity, entityJson["Components"], "LocalTransform");
    serializeIfPresent<Components::Camera>(
    m_registry, entity, entityJson["Components"], "Camera");
    serializeIfPresent<Components::RigidBodyComponent>(
    m_registry, entity, entityJson["Components"], "RigidBodyComponent");
    serializeIfPresent<Components::ColliderComponent>(
    m_registry, entity, entityJson["Components"], "ColliderComponent");
    serializeIfPresent<Components::BoxColliderComponent>(
    m_registry, entity, entityJson["Components"], "BoxColliderComponent");
    serializeIfPresent<Components::CircleColliderComponent>(
    m_registry, entity, entityJson["Components"], "CircleColliderComponent");
    serializeIfPresent<Components::CapsuleColliderComponent>(
    m_registry, entity, entityJson["Components"], "CapsuleColliderComponent");
    serializeIfPresent<Components::Rendering::Renderable>(
    m_registry, entity, entityJson["Components"], "Renderable");
    serializeIfPresent<Components::Rendering::RenderableShape>(
    m_registry, entity, entityJson["Components"], "RenderableShape");
    serializeIfPresent<Components::Rendering::Sprite>(
    m_registry, entity, entityJson["Components"], "Sprite");
    serializeIfPresent<Components::Script>(
    m_registry, entity, entityJson["Components"], "Script");
    serializeIfPresent<Components::MainListener>(
    m_registry, entity, entityJson["Components"], "MainListener");
    serializeIfPresent<Components::NoiseListener>(
    m_registry, entity, entityJson["Components"], "NoiseListener");
    serializeIfPresent<Components::Tags>(
    m_registry, entity, entityJson["Components"], "Tags");
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
    sceneJson["sceneName"] = m_sceneManager.sceneName(sceneID);
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

void SceneSerializer::serializeSceneRuntime(UUID sceneID, const std::string &path) {}

// --------------- Deserialization Helpers ---------------

void SceneSerializer::createEntities(
    const json &sceneJson, std::unordered_map<UUID, entt::entity> &entityMap) {
    for (const auto &entityJson : sceneJson["Entities"]) {
        UUID uuid(entityJson["UUID"].get<UUID>());

        Entity newEntity = m_sceneManager.spawnEntityFromUUIDInScene(
            uuid, UUID(sceneJson["sceneID"].get<UUID>()));
        entityMap[uuid] = newEntity.getHandle();
        newEntity.addTagComponent<Components::HylozoaInternal::Deserializing>();
    }
}

void SceneSerializer::deserializeComponents(
    const json &sceneJson,
    const std::unordered_map<UUID, entt::entity> &entityMap) {
    for (const auto &entityJson : sceneJson["Entities"]) {
        UUID uuid(entityJson["UUID"].get<UUID>());
        entt::entity entity = entityMap.at(uuid);

        const json &components = entityJson["Components"];

        deserializeComponentsIfPresent(entity, components);
    }
}

void SceneSerializer::deserializeComponentsIfPresent(entt::entity entity, const json &componentsJson) {
    deserializeIfPresent<Components::Name>(
    m_registry, entity, componentsJson, "Name");
    deserializeIfPresent<Components::LocalTransform>(
    m_registry, entity, componentsJson, "LocalTransform");
    deserializeIfPresent<Components::Camera>(
    m_registry, entity, componentsJson,"Camera");
    deserializeIfPresent<Components::RigidBodyComponent>(
    m_registry, entity, componentsJson, "RigidBodyComponent");
    deserializeIfPresent<Components::ColliderComponent>(
    m_registry, entity, componentsJson, "ColliderComponent");
    deserializeIfPresent<Components::BoxColliderComponent>(
    m_registry, entity, componentsJson, "BoxColliderComponent");
    deserializeIfPresent<Components::CircleColliderComponent>(
    m_registry, entity, componentsJson, "CircleColliderComponent");
    deserializeIfPresent<Components::CapsuleColliderComponent>(
    m_registry, entity, componentsJson, "CapsuleColliderComponent");
    deserializeIfPresent<Components::Rendering::Renderable>(
    m_registry, entity, componentsJson, "Renderable");
    deserializeIfPresent<Components::Rendering::RenderableShape>(
    m_registry, entity, componentsJson, "RenderableShape");
    deserializeIfPresent<Components::Rendering::Sprite>(
    m_registry, entity, componentsJson, "Sprite");
    deserializeIfPresent<Components::Script>(
    m_registry, entity, componentsJson, "Script");
    deserializeIfPresent<Components::MainListener>(
    m_registry, entity, componentsJson, "MainListener");
    deserializeIfPresent<Components::NoiseListener>(
    m_registry, entity, componentsJson, "NoiseListener");
    deserializeIfPresent<Components::Tags>(
    m_registry, entity, componentsJson, "Tags");
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

void SceneSerializer::deserializeTextures(const json &sceneJson) {
    auto spriteView = m_registry.view<Components::Rendering::Sprite, Components::HylozoaInternal::Deserializing>();

    for (auto entity : spriteView) {
        auto &sprite = spriteView.get<Components::Rendering::Sprite>(entity);
        Entity spriteEntity = Entity::fromHandle(entity, m_registry);
        
        spriteEntity.addComponent<Components::HylozoaInternal::RenderTexture>();
    }
}

void SceneSerializer::deserializeScripts(const json &sceneJson) {
    auto scriptView = m_registry.view<Components::Script, Components::HylozoaInternal::Deserializing>();
    auto& scriptManager = m_registry.ctx().get<ScriptManager>();
    for (auto entity : scriptView) {
        auto& script = scriptView.get<Components::Script>(entity);
        scriptManager.createScriptComponent(script, script.scriptFile, false);
    }
}

UUID SceneSerializer::deserializeScene(const std::string &path) {
    json sceneJson;
    if (!readFromFile(path, sceneJson)) {
        std::cerr << "Failed to read scene file: " << path << std::endl;
        throw std::runtime_error("SceneSerializer::deserializeScene - Failed to read scene file");
    }

    return deserializeScene(sceneJson);
}

UUID SceneSerializer::deserializeScene(const nlohmann::json& sceneJson) {

    if (!sceneJson.contains("sceneID") || !sceneJson.contains("Entities")) {
        std::cerr << "Invalid scene json format" << std::endl;
        throw std::runtime_error("SceneSerializer::deserializeScene (raw json)- Invalid scene json format");
    }

    UUID sceneId = m_sceneManager.createSceneWithUUID(
        sceneJson.value("sceneName", "UnnamedScene"),
        UUID(sceneJson["sceneID"].get<UUID>()));

    if (Hylozoa::Settings::getInstance().getSettings().verbose) {
        std::cout << "Deserializing scene with ID: " << sceneId << std::endl;
    }

    for (const auto &entityJson : sceneJson["Entities"]) {
        if (!entityJson.contains("UUID") || !entityJson.contains("Components")) {
            std::cerr << "Invalid scene json format: each entity must contain 'UUID' and 'Components' fields" << std::endl;
            throw std::runtime_error("SceneSerializer::deserializeScene (raw json) - Invalid scene json format");
        }
    }

    std::unordered_map<UUID, entt::entity> entityMap;

    createEntities(sceneJson, entityMap);
    deserializeComponents(sceneJson, entityMap);
    deserializeRelationships(sceneJson, entityMap);
    deserializeTextures(sceneJson);
    deserializeScripts(sceneJson);
    removeDeserializingTag();
    
    return sceneId;
}

Entity SceneSerializer::deserializePrefab(const std::string& path, const glm::vec2& position) {
    static std::string base = Hylozoa::Settings::getInstance().getSettings().projectLocation;
    std::string fullPath = base + std::string("Assets/") + path;

    json prefabJson;
    if (!readFromFile(fullPath, prefabJson)) {
        std::cerr << "Failed to read prefab file: " << fullPath << std::endl;
        throw std::runtime_error("SceneSerializer::deserializePrefab - Failed to read prefab file");
    }

    return deserializePrefab(prefabJson, position);
}

Entity SceneSerializer::deserializePrefab(const json &entityJson, const glm::vec2& position) 
{
    if (!entityJson.contains("Entities") || !entityJson["Entities"].is_array() || entityJson["Entities"].empty()) {
        std::cerr << "Invalid prefab json format: missing 'Entities' field" << std::endl;
        throw std::runtime_error("SceneSerializer::deserializePrefab - Invalid prefab json format");
    }

    auto& entities = entityJson["Entities"];
    std::unordered_map<int, entt::entity> entityMap;

    for (const auto& entity : entities) {
        if (!entity.contains("Components") || !entity["Components"].is_object()) {
            std::cerr << "Invalid prefab json format: each entity must contain a 'Components' object" << std::endl;
            throw std::runtime_error("SceneSerializer::deserializePrefab - Invalid prefab json format");
        }
        if (!entity.contains("id") || !entity["id"].is_number_integer()) {
            std::cerr << "Invalid prefab json format: each entity must contain an integer 'id' field (root entity MUST be id = 0)" << std::endl;
            throw std::runtime_error("SceneSerializer::deserializePrefab - Invalid prefab json format");
        }

        Entity newEntity = m_sceneManager.spawnEntity();
        newEntity.addTagComponent<Components::HylozoaInternal::Deserializing>();
        int prefabEntityId = entity["id"].get<int>();
        entityMap[prefabEntityId] = newEntity.getHandle();
        deserializeComponentsIfPresent(newEntity.getHandle(), entity["Components"]);
    }

    deserializeRelationshipsPrefab(entities, entityMap);
    deserializeTextures(entityJson);
    deserializeScripts(entityJson);
    removeDeserializingTag();

    try {
        auto rootEntity = Entity::fromHandle(entityMap.at(0), m_registry);
        auto& localTransform = rootEntity.getComponent<Components::LocalTransform>();
        localTransform.position = position;
        return rootEntity;
    } catch (const std::out_of_range& ex) {
        std::cerr << "Error setting prefab position: Root entity with id 0 not found. " << ex.what() << std::endl;
    } catch (const std::exception& ex) {
        std::cerr << "Error setting prefab position: " << ex.what() << std::endl;
    }

    throw std::runtime_error("SceneSerializer::deserializePrefab - Failed to generate prefab entity from json");
}

void SceneSerializer::deserializeRelationshipsPrefab(const json &entityJson,
    const std::unordered_map<int, entt::entity> &entityMap) {

    if (entityMap.size() < 2) return;

    for (const auto& entity : entityJson) {
        if (!entity.contains("Parent") || entity["Parent"].is_null() || !entity["Parent"].is_number_integer()) {
            continue;
        }

        int childId = entity["id"].get<int>();
        int parentId = entity["Parent"].get<int>();

        try {
            entt::entity child = entityMap.at(childId);
            entt::entity parent = entityMap.at(parentId);
            Entity childEntity = Entity::fromHandle(child, m_registry);
            childEntity.childOf(parent);
        } catch (const std::out_of_range& ex) {
            std::cerr << "SceneSerializer::deserializeRelationshipsPrefab - Error deserializing prefab relationships: Entity with id " << ex.what() << " not found in entity map." << std::endl;
            continue;
        } catch (const std::exception& ex) {
            std::cerr << "SceneSerializer::deserializeRelationshipsPrefab - Error deserializing prefab relationships: " << ex.what() << std::endl;
            continue;
        }
    }
}

void SceneSerializer::removeDeserializingTag() {
    auto view = m_registry.view<Components::HylozoaInternal::Deserializing>();

    for (auto entity : view) {
        m_registry.remove<Components::HylozoaInternal::Deserializing>(entity);
    }
}

void SceneSerializer::deserializeSceneRuntime(uint64_t sceneID,
                                              const std::string &path) {
}

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