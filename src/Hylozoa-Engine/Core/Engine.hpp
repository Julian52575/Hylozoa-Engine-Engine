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
#include "Input.hpp"
#include <entt/entt.hpp>
#include <iostream>

namespace Hylozoa {

enum class EngineMode { Normal, Headless };

class Engine {
public:
  Engine(EngineMode mode = EngineMode::Normal);
  ~Engine() = default;

  entt::registry &getRegistry() { return m_registry; }
  void stop();
  void pause();
  void runTick(int tick = 1);
  void runTick(float realDelta);
  void run();
  void onUpdate(float deltaTime);
  void fixedUpdate(float fixedDeltaTime);

  void updateTime();

  // temp
  void beginFrame() { m_inputManager.beginFrame(); }
  InputManager input() { return m_inputManager; }

  // This Will not be handled by the Engine and will be moved to a Scene Manager
  // later
  Entity createEntity(const std::string &name = "");
  Entity createSpacialEntity(const std::string &name = "");

private:
  entt::registry m_registry;
  SystemManager m_systemManager{m_registry};
  InputManager m_inputManager{m_registry};
};

} // namespace Hylozoa

#endif /* !ENGINE_HPP_ */