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
  auto entity = Entity{this->m_registry.create(), m_registry};
  if (name != "") {
    entity.add_component<Name>(Name{name});
  }
  return entity;
}

Entity Engine::createSpacialEntity(const std::string &name) {
  auto entity = Entity{this->m_registry.create(), m_registry};

  if (name != "") {
    entity.add_component<Name>(Name{name});
  }

  entity.add_component<LocalTransform>(
      LocalTransform{{0.0f, 0.0f}, {1.0f, 1.0f}, 0.0f});
  return entity;
}
} // namespace Hylozoa