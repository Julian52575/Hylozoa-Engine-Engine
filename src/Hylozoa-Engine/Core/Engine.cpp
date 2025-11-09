/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Heart Class of the Hylozoa Engine [source file]
*/
#include "Engine.hpp"
#include "Hylozoa-Engine/Systems/Physics/Collision.hpp"
#include "Hylozoa-Engine/Systems/Renderer/Renderer.hpp"
#include "Hylozoa-Engine/Systems/Transform/Transform.hpp"

#include <chrono>

namespace Hylozoa {

Engine::Engine() {
  std::cout << "[Engine] Initializing Hylozoa Engine..." << std::endl;

  m_systemManager.registerSystem<ParentChildSystem>(0);
  m_systemManager.registerSystem<UpdateTransformSystem>(1);
  m_systemManager.registerFixedSystem<CollisionSystem>(0);
  m_systemManager.registerSystem<Systems::Renderer>(99);

  m_systemManager.orderAllSystems();
  m_systemManager.startAll();

  std::cout << "[Engine] Hylozoa Engine initialized." << std::endl;
}

void Engine::run() {
  using clock = std::chrono::high_resolution_clock;
  auto previous = clock::now();
  m_isRunning = true;
  m_accumulator = 0.0;

  while (m_isRunning) {
    auto current = clock::now();
    std::chrono::duration<double> elapsed = current - previous;
    previous = current;
    m_accumulator += elapsed.count();

    while (m_accumulator >= FIXED_DELTA) {
      FixedUpdate(FIXED_DELTA); // Physics
      m_accumulator -= FIXED_DELTA;
    }

    OnUpdate(static_cast<float>(elapsed.count())); // normal Update

    // Render
  }
}

void Engine::runTick(int tick) {
  for (int i = 0; i < tick; ++i) {
    m_systemManager.updateAll(FIXED_DELTA);
  }
}

void Engine::FixedUpdate(float fixedDeltaTime) {
  m_systemManager.updateFixed(fixedDeltaTime);
}

void Engine::OnUpdate(float deltaTime) { m_systemManager.update(deltaTime); }

Entity Engine::createEntity(const std::string &name) {
  auto entity = Entity{this->m_registry.create(), m_registry};
  if (name != "") {
    entity.addComponent<Name>(Name{name});
  }
  return entity;
}

Entity Engine::createSpacialEntity(const std::string &name) {
  auto entity = Entity{this->m_registry.create(), m_registry};

  if (name != "") {
    entity.addComponent<Name>(Name{name});
  }

  entity.addComponent<LocalTransform>(
      LocalTransform{{0.0f, 0.0f}, {1.0f, 1.0f}, 0.0f});
  return entity;
}
} // namespace Hylozoa