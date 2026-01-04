/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Scene Core [source file]
*/

#include "Hylozoa-Engine/Core/Scene.hpp"
#include "Hylozoa-Engine/Components/Context/Events.hpp"
#include "Hylozoa-Engine/Components/Scene/Scene.hpp"
#include "Hylozoa-Engine/Components/Context/SceneState.hpp"
#include "Hylozoa-Engine/Components/Scene/UUID.hpp"

#include <algorithm>
#include <iostream>

namespace Hylozoa {

// -------------- Scene Class

Entity Scene::spawnEntityInScene(std::string& name, entt::registry& registry) {
  auto newEntity = Entity{registry.create(), registry};
  newEntity.addComponent<Components::Name>(name);
  newEntity.addComponent<Components::LocalTransform>(Components::LocalTransform{{0.0f, 0.0f}, {1.0f, 1.0f}, 0.0f});
  newEntity.addComponent<Components::HylozoaInternal::SceneTag>(Components::HylozoaInternal::SceneTag{m_name, m_id});
  newEntity.addComponent<Components::HylozoaInternal::Id>(UUID());
  newEntity.addTag<Components::HylozoaInternal::SceneActiveTag>();

  std::cout << "Spawned entity '" << name << "' in scene '" << m_name << "' (ID: " << m_id << ")." << std::endl;
  return newEntity;
}

// -------------- SceneManager Class

SceneManager::SceneManager(entt::registry& registry)
  : m_registry(registry)
{
}

Entity SceneManager::spawnEntity(std::string name) {
  if (m_loadedScenes.empty()) {
    throw std::runtime_error("No scene loaded to spawn entity in.");
  }
  if (name.empty()) {
    name = "Entity";
  }
  uint64_t topLoadedSceneId = m_loadedScenes.back();
  auto& scene = m_scenesById[topLoadedSceneId];

  return scene->spawnEntityInScene(name, m_registry);
}

Entity SceneManager::spawnEntityInScene(std::string name, uint64_t sceneID) {
  auto it = m_scenesById.find(sceneID);
  if (it == m_scenesById.end()) {
    throw std::runtime_error("Scene with ID " + std::to_string(sceneID) + " does not exist.");
  }
  if (name.empty()) {
    name = "Entity";
  }
  auto& scene = it->second;

  return scene->spawnEntityInScene(name, m_registry);
}

void SceneManager::initialize() {
  loadScene(createScene("DefaultScene"));
}

uint64_t SceneManager::createScene(const std::string &name) {
  auto &sceneState = m_registry.ctx().get<Components::HylozoaInternal::SceneState>();

  uint64_t newId = m_scenesById.size() + 1;
  m_scenesById[newId] = std::make_unique<Scene>(newId, name);

  sceneState.states[newId] = Components::HylozoaInternal::SceneState::State::UNLOADED;

  return newId;
}

void SceneManager::loadScene(const std::string &name) {
  for (const auto& [knownId, scene] : m_scenesById) {
    if (scene->name() == name) {
      activateScene(knownId);
      return;
    }
  }

  throw std::runtime_error("Scene not found: " + name);
}

void SceneManager::loadScene(const uint64_t id) {
  for (const auto& [knownId, scene] : m_scenesById) {
    if (knownId == id) {
      activateScene(knownId);
      return;
    }
  }
  throw std::runtime_error("Scene not found with ID: " + std::to_string(id));
}

void SceneManager::unloadScene(const std::string &name) {
  auto &sceneState = m_registry.ctx().get<Components::HylozoaInternal::SceneState>();

  for (const auto& [knownId, scene] : m_scenesById) {
    if (scene->name() == name) {
      auto sceneToUnloadIt = std::find(m_loadedScenes.begin(), m_loadedScenes.end(), knownId);
      if (sceneToUnloadIt != m_loadedScenes.end()) {
        m_loadedScenes.erase(sceneToUnloadIt);
        deactivateScene(knownId);
        return;
      }
    }
  }

  throw std::runtime_error("Scene do not exist or not loaded: " + name);
}

void SceneManager::unloadScene(const uint64_t id) {
  auto &sceneState = m_registry.ctx().get<Components::HylozoaInternal::SceneState>();

  auto sceneToUnloadIt = std::find(m_loadedScenes.begin(), m_loadedScenes.end(), id);
  if (sceneToUnloadIt != m_loadedScenes.end()) {
    m_loadedScenes.erase(sceneToUnloadIt);
    deactivateScene(id);
    return;
  }

  throw std::runtime_error("Scene do not exist or not loaded (id): " + std::to_string(id));
}

void SceneManager::activateScene(const uint64_t id) {
  auto &sceneState = m_registry.ctx().get<Components::HylozoaInternal::SceneState>();
  auto &dispatcher = m_registry.ctx().get<Components::HylozoaInternal::EventsDispatcher>();
  auto view = m_registry.view<Components::HylozoaInternal::SceneTag>();

  for (auto entity : view) {
    if (view.get<Components::HylozoaInternal::SceneTag>(entity).id == id) {
      m_registry.emplace<Components::HylozoaInternal::SceneActiveTag>(entity);
    }
  }

  m_loadedScenes.push_back(id);
  sceneState.states[id] = Components::HylozoaInternal::SceneState::State::LOADED;
  dispatcher.dispatcher.trigger<Components::HylozoaInternal::OnSceneLoaded>(Components::HylozoaInternal::OnSceneLoaded{id});
  std::cout << "Scene with ID " << id << " loaded." << std::endl;
}

void SceneManager::deactivateScene(const uint64_t id) {
  auto &sceneState = m_registry.ctx().get<Components::HylozoaInternal::SceneState>();
  auto &dispatcher = m_registry.ctx().get<Components::HylozoaInternal::EventsDispatcher>();
  auto view = m_registry.view<Components::HylozoaInternal::SceneTag, Components::HylozoaInternal::SceneActiveTag>();

  for (auto entity : view) {
    if (view.get<Components::HylozoaInternal::SceneTag>(entity).id == id) {
      m_registry.remove<Components::HylozoaInternal::SceneActiveTag>(entity);
    }
  }

  sceneState.states[id] = Components::HylozoaInternal::SceneState::State::UNLOADED;
  std::cout << "Scene with ID " << id << " unloaded." << std::endl;
  dispatcher.dispatcher.trigger<Components::HylozoaInternal::OnSceneUnloaded>(Components::HylozoaInternal::OnSceneUnloaded{id});
}

// -------------- SceneSerializer Class

void SceneSerializer::serializeScene(uint64_t sceneID, const std::string &path) {
  // Implementation for serializing the scene to a file at 'path'
}
void SceneSerializer::serializeSceneRuntime(uint64_t sceneID, const std::string &path) {
  // Implementation for serializing the scene runtime state to a file at 'path'
}
void SceneSerializer::deserializeScene(uint64_t sceneID, const std::string &path) {
  // Implementation for deserializing the scene from a file at 'path'
}
void SceneSerializer::deserializeSceneRuntime(uint64_t sceneID, const std::string &path) {
  // Implementation for deserializing the scene runtime state from a file at 'path'
}

} // namespace Hylozoa