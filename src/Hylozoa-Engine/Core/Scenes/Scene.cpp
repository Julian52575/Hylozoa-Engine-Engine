/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Scene Core [source file]
*/

#include "SceneSerializer.hpp"

#include "Hylozoa-Engine/Components/Context/Events.hpp"
#include "Hylozoa-Engine/Components/Context/SceneState.hpp"
#include "Hylozoa-Engine/Components/Scene/Scene.hpp"
#include "Hylozoa-Engine/Components/Scene/UUID.hpp"

#include "Hylozoa-Engine/Core/Scenes/Scene.hpp"
#include "Hylozoa-Engine/Core/Settings.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>

#include <nlohmann/json.hpp>

// for convenience
using json = nlohmann::json;

namespace Hylozoa {

// -------------- Scene Class

Entity Scene::spawnEntityInScene(std::string &name, entt::registry &registry) {
    auto newEntity = spawnEntityFromUUID(UUID(), registry);
    newEntity.addComponent<Components::Name>(name);
    newEntity.addComponent<Components::LocalTransform>(
        Components::LocalTransform{{0.0f, 0.0f}, {1.0f, 1.0f}, 0.0f});

    if (Hylozoa::Settings::getInstance().getSettings().verbose) {
        std::cout << "Spawned entity '" << name << "' in scene '" << m_name
                  << "' (ID: " << m_id << ")." << std::endl;
    }
    return newEntity;
}

Entity Scene::spawnRawEntity(entt::registry &registry) {
    auto rawEntity = Entity{registry.create(), registry};

    rawEntity.addComponent<Components::HylozoaInternal::SceneTag>(
        Components::HylozoaInternal::SceneTag{m_name, m_id});

    rawEntity.addTagComponent<Components::HylozoaInternal::SceneActiveTag>();

    return rawEntity;
}

Entity Scene::spawnEntityFromUUID(UUID uuid, entt::registry &registry) {
    Entity newEntity = spawnRawEntity(registry);

    newEntity.addComponent<Components::HylozoaInternal::Id>(uuid);

    return newEntity;
}

void Scene::destroyScene(entt::registry &registry) {
    auto view = registry.view<Components::HylozoaInternal::SceneTag>();

    for (auto entity : view) {
        auto &sceneTag = view.get<Components::HylozoaInternal::SceneTag>(entity);
        if (sceneTag.id == m_id) {
            auto toDelete = Entity::fromHandle(entity, registry);
            if (Hylozoa::Settings::getInstance().getSettings().verbose) {
                std::cout << "Destroyed entity with Name '" << toDelete.getName()
                        << "' from scene '" << m_name << "' (scene ID: " << m_id
                        << ")." << std::endl;
            }
            toDelete.destroy();
        }
    }
}

// -------------- SceneManager Class

SceneManager::SceneManager(entt::registry &registry) : m_registry(registry) {}

Entity SceneManager::spawnEntity(std::string name) {
    if (m_loadedScenes.empty()) {
        throw std::runtime_error("No scene loaded to spawn entity in.");
    }
    if (name.empty()) {
        name = "Entity";
    }
    UUID topLoadedSceneId = m_loadedScenes.back();
    auto &scene = m_scenesById[topLoadedSceneId];

    return scene->spawnEntityInScene(name, m_registry);
}

Entity SceneManager::spawnEntityInScene(std::string name, UUID sceneID) {
    auto it = m_scenesById.find(sceneID);
    if (it == m_scenesById.end()) {
        throw std::runtime_error("Scene with ID " + std::to_string(sceneID) +
                                 " does not exist.");
    }
    if (name.empty()) {
        name = "Entity";
    }
    auto &scene = it->second;

    auto sceneState =
        m_registry.ctx().get<Components::HylozoaInternal::SceneState>();
    if (sceneState.states[sceneID] !=
        Components::HylozoaInternal::SceneState::State::LOADED) {
        throw std::runtime_error("Cannot spawn entity: scene " +
                                 std::to_string(sceneID) + " not loaded.");
    }

    return scene->spawnEntityInScene(name, m_registry);
}

Entity SceneManager::spawnEntityFromUUIDInScene(UUID uuid, UUID sceneID) {
    auto it = m_scenesById.find(sceneID);
    if (it == m_scenesById.end()) {
        throw std::runtime_error("Cannot deserialize entity: scene " +
                                 std::to_string(sceneID) + " not loaded.");
    }

    auto &scene = it->second;

    return scene->spawnEntityFromUUID(uuid, m_registry);
}

void SceneManager::initialize() { }

UUID SceneManager::createScene(const std::string &name) {
    auto &sceneState =
        m_registry.ctx().get<Components::HylozoaInternal::SceneState>();

    UUID newId;
    m_scenesById[newId] = std::make_unique<Scene>(newId, name);

    sceneState.states[newId] =
        Components::HylozoaInternal::SceneState::State::UNLOADED;

    return newId;
}

UUID SceneManager::createSceneWithUUID(const std::string &name, UUID uuid) {
    auto &sceneState =
        m_registry.ctx().get<Components::HylozoaInternal::SceneState>();

    if (m_scenesById.contains(uuid)) {
        throw std::runtime_error("Scene with this UUID already exists");
    }

    m_scenesById[uuid] = std::make_unique<Scene>(uuid, name);

    sceneState.states[uuid] =
        Components::HylozoaInternal::SceneState::State::UNLOADED;

    return uuid;
}

void SceneManager::destroyScene(const std::string& name)
{
    auto &sceneState =
        m_registry.ctx().get<Components::HylozoaInternal::SceneState>();

    for (const auto &[knownId, scene] : m_scenesById) {
        if (scene->name() == name) {
            if (sceneState.states[knownId] == Components::HylozoaInternal::SceneState::State::LOADED) {
                unloadScene(knownId);
            }
            scene->destroyScene(m_registry);
            m_loadedScenes.erase(std::find(m_loadedScenes.begin(), m_loadedScenes.end(), knownId));
            m_scenesById.erase(knownId);
            sceneState.states.erase(knownId);
            return;
        }
    }

    throw std::runtime_error("SceneManager::destroyScene (name) - Scene not found: " + name);
}

void SceneManager::destroyScene(const UUID id)
{
    auto &sceneState =
        m_registry.ctx().get<Components::HylozoaInternal::SceneState>();

    auto it = m_scenesById.find(id);
    if (it != m_scenesById.end()) {
        if (sceneState.states[id] == Components::HylozoaInternal::SceneState::State::LOADED) {
            unloadScene(id);
        }
        it->second->destroyScene(m_registry);
        m_loadedScenes.erase(std::find(m_loadedScenes.begin(), m_loadedScenes.end(), id));
        m_scenesById.erase(it);
        sceneState.states.erase(id);
        return;
    }

    throw std::runtime_error("SceneManager::destroyScene (id) - Scene not found: " + std::to_string(id));
}

void SceneManager::clearScenes() {
    auto &sceneState =
        m_registry.ctx().get<Components::HylozoaInternal::SceneState>();

    for (const auto &[knownId, scene] : m_scenesById) {
        if (sceneState.states[knownId] == Components::HylozoaInternal::SceneState::State::LOADED) {
            unloadScene(knownId);
        }
        scene->destroyScene(m_registry);
    }
    m_scenesById.clear();
    m_loadedScenes.clear();
    sceneState.states.clear();
}

void SceneManager::loadScene(const std::string &name) {
    for (const auto &[knownId, scene] : m_scenesById) {
        if (scene->name() == name) {
            activateScene(knownId);
            return;
        }
    }

    throw std::runtime_error("SceneManager::loadScene (name) - Scene not found: " + name);
}

void SceneManager::loadScene(const UUID id) {
    for (const auto &[knownId, scene] : m_scenesById) {
        if (knownId == id) {
            activateScene(knownId);
            return;
        }
    }
    throw std::runtime_error("SceneManager::loadScene (id) - Scene not found: " + std::to_string(id));
}

void SceneManager::unloadScene(const std::string &name) {
    auto &sceneState =
        m_registry.ctx().get<Components::HylozoaInternal::SceneState>();

    for (const auto &[knownId, scene] : m_scenesById) {
        if (scene->name() == name) {
            auto sceneToUnloadIt = std::find(m_loadedScenes.begin(),
                                             m_loadedScenes.end(), knownId);
            if (sceneToUnloadIt != m_loadedScenes.end()) {
                m_loadedScenes.erase(sceneToUnloadIt);
                deactivateScene(knownId);
                return;
            }
        }
    }

    throw std::runtime_error("SceneManager::unloadScene - Scene do not exist or not loaded: " + name);
}

void SceneManager::unloadScene(const UUID id) {
    auto &sceneState =
        m_registry.ctx().get<Components::HylozoaInternal::SceneState>();

    auto sceneToUnloadIt =
        std::find(m_loadedScenes.begin(), m_loadedScenes.end(), id);
    if (sceneToUnloadIt != m_loadedScenes.end()) {
        m_loadedScenes.erase(sceneToUnloadIt);
        deactivateScene(id);
        return;
    }

    throw std::runtime_error("Scene do not exist or not loaded (id): " +
                             std::to_string(id));
}

void SceneManager::activateScene(const UUID id) {
    auto &sceneState =
        m_registry.ctx().get<Components::HylozoaInternal::SceneState>();
    auto &dispatcher =
        m_registry.ctx().get<Components::HylozoaInternal::EventsDispatcher>();
    auto view = m_registry.view<Components::HylozoaInternal::SceneTag>();

    for (auto entity : view) {
        if (view.get<Components::HylozoaInternal::SceneTag>(entity).id == id) {
            m_registry.emplace_or_replace<
                Components::HylozoaInternal::SceneActiveTag>(entity);
        }
    }

    m_loadedScenes.push_back(id);
    sceneState.states[id] =
        Components::HylozoaInternal::SceneState::State::LOADED;
    dispatcher.dispatcher.trigger<Components::HylozoaInternal::OnSceneLoaded>(
        Components::HylozoaInternal::OnSceneLoaded{id});
    if (Hylozoa::Settings::getInstance().getSettings().verbose) {
        std::cout << "Scene with ID " << id.value() << " loaded." << std::endl;
    }
}

void SceneManager::deactivateScene(const UUID id) {
    auto &sceneState =
        m_registry.ctx().get<Components::HylozoaInternal::SceneState>();
    auto &dispatcher =
        m_registry.ctx().get<Components::HylozoaInternal::EventsDispatcher>();
    auto view = m_registry.view<Components::HylozoaInternal::SceneTag,
                                Components::HylozoaInternal::SceneActiveTag>();

    for (auto entity : view) {
        if (view.get<Components::HylozoaInternal::SceneTag>(entity).id == id) {
            m_registry.remove<Components::HylozoaInternal::SceneActiveTag>(
                entity);
        }
    }

    sceneState.states[id] =
        Components::HylozoaInternal::SceneState::State::UNLOADED;
    if (Hylozoa::Settings::getInstance().getSettings().verbose) {
        std::cout << "Scene with ID " << id.value() << " unloaded."
                  << std::endl;
    }
    dispatcher.dispatcher.trigger<Components::HylozoaInternal::OnSceneUnloaded>(
        Components::HylozoaInternal::OnSceneUnloaded{id});
}

} // namespace Hylozoa