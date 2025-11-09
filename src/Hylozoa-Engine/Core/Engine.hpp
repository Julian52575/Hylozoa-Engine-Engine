/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Heart Class of the Hylozoa Engine [header]
*/

#ifndef ENGINE_HPP_
#define ENGINE_HPP_

#include "Entity.hpp"
#include "Hylozoa-Engine/Systems/Manager/SystemManager.hpp"
#include <entt/entt.hpp>
#include <iostream>

namespace Hylozoa {

enum class EngineState { RUNNING, PAUSED, STOPPED };

class Engine {
public:
  Engine();
  ~Engine() = default;

  entt::registry &getRegistry() { return m_registry; }
  void stop() { m_isRunning = false; }
  void runTick(int tick = 1);
  void run();
  void OnUpdate(float deltaTime);
  void FixedUpdate(float fixedDeltaTime);

  // This Will not be handled by the Engine and will be moved to a Scene Manager
  // later
  Entity createEntity(const std::string &name = "");
  Entity createSpacialEntity(const std::string &name = "");

private:
  entt::registry m_registry;
  bool m_isRunning = false;
  EngineState m_state = EngineState::STOPPED;
  SystemManager m_systemManager{m_registry};

  double m_accumulator = 0.0;
  const float FIXED_DELTA = 1.0f / 60.0f;
};

} // namespace Hylozoa

#endif /* !ENGINE_HPP_ */