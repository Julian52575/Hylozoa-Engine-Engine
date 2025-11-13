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

  Entity createEntity(const std::string &name = "");
  Entity createSpacialEntity(const std::string &name = "");

  template <typename T>
  T& addComponentToEntity(Entity entity,const T& component) {
      return m_registry.emplace<T>(entity.get_internal_entity(), component);
  }
  template <typename T, typename... Args>
  T& addComponentToEntity(Entity entity, Args&&... args) {
      return m_registry.emplace<T>(entity.get_internal_entity(), std::forward<Args>(args)...);
  }


private:
  entt::registry m_registry;
  bool m_isRunning = false;
  EngineState m_state = EngineState::STOPPED;
};

} // namespace Hylozoa

#endif /* !ENGINE_HPP_ */