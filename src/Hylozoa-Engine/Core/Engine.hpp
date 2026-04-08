/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Heart Class of the Hylozoa Engine [header]
*/

#ifndef ENGINE_HPP_
#define ENGINE_HPP_

#include "Audio.hpp"
#include "Entity.hpp"
#include "Hylozoa-Engine/Systems/Manager/SystemManager.hpp"
#include "Input.hpp"
#include "Scene.hpp"
#include "Time.hpp"
#include <entt/entt.hpp>
#include <iostream>

namespace Hylozoa {

/**
 * @enum EngineMode
 * @brief Defines the mode in which the engine operates.
 *
 * This enum is used to specify whether the engine should run in normal mode
 * with rendering, or in headless mode without rendering (useful for server-side
 * applications or automated testing).
 */
enum class EngineMode { NORMAL, HEADLESS };

/**
 * @class Engine
 * @brief The core class of the Hylozoa Engine.
 *
 * This class is the core of the engine, you can access all managers through it.
 */
class Engine {
  public:
    /**
     * @brief Construct a new Engine object.
     * @param mode The mode to run the engine in (normal or headless).
     */
    Engine(EngineMode mode = EngineMode::NORMAL);

    /**
     * @brief Construct a new Engine object
     * 
     * @param settingsJsonPath the file path of the settings data.
     * 
     * @note This constructor defaults to NORMAL mode when a settings file is provided.
     */
    Engine(const std::string& settingsJsonPath);

    /**
     * @brief Construct a new Engine object.
     * @param mode The mode to run the engine in (normal or headless).
     * @param settingsJsonPath The file path of the settings data.
     */
    Engine(EngineMode mode, const std::string &settingsJsonPath);

    /**
     * @brief Construct a new Engine object.
     * @param mode The mode to run the engine in (normal or headless).
     * @param jsonStream The input stream containing the settings data.
     */
    Engine(EngineMode mode, std::istream &jsonStream);

    ~Engine() = default;
    /**
     * @brief Get the entt registry used by the engine.
     *
     * @return entt::registry&
     */
    entt::registry &getRegistry() { return m_registry; }
    /**
     * @brief Get the Input Manager of the engine.
     *
     * @return Input&
     */
    Input &input() { return m_inputManager; }
    /**
     * @brief Get the Time Manager of the engine.
     *
     * @return Time&
     */
    Time &time() { return m_timeManager; }
    /**
     * @brief Get the Scene Manager of the engine.
     *
     * @return SceneManager&
     */
    SceneManager &scene() { return m_sceneManager; }
    /**
     * @brief Get the Audio Manager of the engine.
     *
     * @return Audio&
     */
    Audio &audio() { return m_audioManager; }

    // Stop the engine
    void stop();
    // Pause the engine
    void pause();
    // Unpause the engine
    void unpause();

    /**
     * @brief run a given number of ticks with a fixed delta time.
     *
     * @param tick  the number of ticks to run (default is 1)
     */
    void runTick(int tick = 1);
    /**
     * @brief run a single tick with a specified real delta time, allowing for
     * variable time steps in the update loop.
     *
     * @param realDelta  the real delta time to use for the tick, in seconds
     */
    void runTick(float realDelta);
    // Main engine loop
    void run();

    // clear input states at the beginning of each frame
    void beginFrame() { m_inputManager.beginFrame(); }

    /**
     * @brief Initialize the engine with loaded settings.
     * Initialize all internal systems and managers of the engine, preparing it for the main loop.
     *
     * This function NEEDS to be called after construction.
     */
    void init();
  private:
    // main initialization function, called by all constructors
    EngineMode mode = EngineMode::NORMAL;
    entt::registry m_registry;
    SystemManager m_systemManager{m_registry};
    SceneManager m_sceneManager{m_registry};
    Input m_inputManager{m_registry};
    Time m_timeManager{m_registry};
    Audio m_audioManager{m_registry};
    
  private:
    void onUpdate(float deltaTime);
    void fixedUpdate(float fixedDeltaTime);

    void loadSettings(const std::string &settingsPath = "src/settings.json");
    void loadSettings(std::istream &jsonStream);
};

} // namespace Hylozoa

#endif /* !ENGINE_HPP_ */