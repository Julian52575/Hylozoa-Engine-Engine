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
#include "Time.hpp"
#include <entt/entt.hpp>
#include <iostream>

namespace Hylozoa {

enum class EngineMode { Normal, Headless };

class Engine {
  public:
    Engine(EngineMode mode = EngineMode::Normal);
    ~Engine() = default;

    // Get registry
    entt::registry &getRegistry() { return m_registry; }
    // Get Input Manager
    Input input() { return m_inputManager; }

    // Stop the engine
    void stop();
    // Pause the engine
    void pause();

    // Run a given number of ticks (fixed update)
    void runTick(int tick = 1);
    // Run a tick with a given real delta time
    void runTick(float realDelta);
    // Main engine loop
    void run();

    // temp

    // clear input states at the beginning of each frame
    void beginFrame() { m_inputManager.beginFrame(); }

    // This Will not be handled by the Engine and will be moved to a Scene
    // Manager later
    Entity createEntity(const std::string &name = "");
    Entity createSpacialEntity(const std::string &name = "");

  private:
    entt::registry m_registry;
    SystemManager m_systemManager{m_registry};
    Input m_inputManager{m_registry};
    Time m_timeManager{m_registry};

    void onUpdate(float deltaTime);
    void fixedUpdate(float fixedDeltaTime);
};

} // namespace Hylozoa

#endif /* !ENGINE_HPP_ */