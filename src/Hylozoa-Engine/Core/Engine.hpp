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
#include "Scene.hpp"
#include "Time.hpp"
#include "Audio.hpp"
#include <entt/entt.hpp>
#include <iostream>

namespace Hylozoa {

enum class EngineMode { NORMAL, HEADLESS };

/*
 * @class Engine
 * @brief The core class of the Hylozoa Engine.
 *
 * This class is the core of the engine, you can access all managers through it.
 */
class Engine {
  public:
    Engine(EngineMode mode = EngineMode::NORMAL);
    ~Engine() = default;

    // Get registry
    entt::registry &getRegistry() { return m_registry; }
    // Get Input Manager
    Input &input() { return m_inputManager; }
    // Get Time Manager
    Time &time() { return m_timeManager; }
    // Get Scene Manager
    SceneManager &scene() { return m_sceneManager; }
    // Get Audio Manager
    Audio &audio() { return m_audioManager; }

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

  private:
    entt::registry m_registry;
    SystemManager m_systemManager{m_registry};
    SceneManager m_sceneManager{m_registry};
    Input m_inputManager{m_registry};
    Time m_timeManager{m_registry};
    Audio m_audioManager{m_registry};
  private:
    void onUpdate(float deltaTime);
    void fixedUpdate(float fixedDeltaTime);
};

} // namespace Hylozoa

#endif /* !ENGINE_HPP_ */