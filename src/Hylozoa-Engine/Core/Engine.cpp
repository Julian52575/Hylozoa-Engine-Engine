/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Heart Class of the Hylozoa Engine [source file]
*/
#include "Engine.hpp"
#include "Hylozoa-Engine/Systems/Transform.hpp"

namespace Hylozoa {

Engine::Engine() {
  std::cout << "[Engine] Initializing Hylozoa Engine..." << std::endl;

  // Register systems here

  std::cout << "[Engine] Hylozoa Engine initialized." << std::endl;
}

void Engine::run() {
  this->m_isRunning = true;

  while (this->m_isRunning) {
    OnUpdate();
  }
}

void Engine::runTick(int tick) {
  for (int i = 0; i < tick; ++i) {
    this->OnUpdate();
  }
}

void Engine::OnUpdate() {
  parent_child_system(this->m_registry);
  local_to_world_system(this->m_registry);
}

Entity Engine::createEntity(const std::string &name) {
  auto entity = this->m_registry.create();
  if (name != "") {
    this->m_registry.emplace<Name>(entity, name);
  }
  return Entity{entity};
}

Entity Engine::createSpacialEntity(const std::string &name) {
  auto entity = this->m_registry.create();

  if (name != "") {
    this->m_registry.emplace<Name>(entity, name);
  }

  this->m_registry.emplace<LocalTransform>(entity,
                                           LocalTransform{{0, 0}, {1, 1}, 0});
  return Entity{entity};
}
} // namespace Hylozoa