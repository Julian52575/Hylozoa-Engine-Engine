/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Heart Class of the Hylozoa Engine [header]
*/

#ifndef ENGINE_HPP_
#define ENGINE_HPP_

#include "Entity.hpp"
#include <entt/entt.hpp>
#include <iostream>

namespace Hylozoa {

enum class EngineState { RUNNING, PAUSED, STOPPED };

class Engine {
public:
  Engine();
  ~Engine() = default;

  entt::registry &get_registry() { return m_registry; }
  void stop() { m_isRunning = false; }
  void runTick(int tick = 1);
  void run();
  void OnUpdate();

  // This Will not be handled by the Engine and will be moved to a Scene Manager
  // later
  Entity createEntity(const std::string &name = "");
  Entity createSpacialEntity(const std::string &name = "");

private:
  entt::registry m_registry;
  bool m_isRunning = false;
  EngineState m_state = EngineState::STOPPED;
};

} // namespace Hylozoa

#endif /* !ENGINE_HPP_ */