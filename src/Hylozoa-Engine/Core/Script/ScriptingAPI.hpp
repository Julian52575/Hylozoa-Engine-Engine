/*
** EPITECH PROJECT, 2026
** Hylozoa Engine
** File description:
** Scripting API [header]
*/

#ifndef SCRIPTING_API_HPP_
#define SCRIPTING_API_HPP_

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>
#include <string>

#include "Hylozoa-Engine/Core/Entities/Entity.hpp"
#include "Hylozoa-Engine/Core/IO/Input.hpp"
#include "Hylozoa-Engine/Core/Scenes/Scene.hpp"

namespace Hylozoa {

/**
 * @brief Scripting API - exposes engine functionalities to Lua scripts
 *
 * The Scripting API class provides a set of functions that can be called from
 * Lua scripts to interact with the engine. It serves as a bridge between the
 * Lua scripting environment and the C++ engine, allowing scripts to access and
 * manipulate game entities, input states, scene management functionalities and
 * such.
 */
class ScriptingAPI {
  public:
    ScriptingAPI(entt::registry &registry, sol::state &lua);
    ~ScriptingAPI() = default;

    void initialize();

    // ------------ Utility API ------------
    void yolo();
    /**
     * @brief allows Lua scripts to log messages to the console with a
     * consistent format.
     *
     * the usual lua print function is redirected to this function.
     *
     * @param va
     */
    void log_message(sol::variadic_args va);

    // ------------ Entity API ------------
    /**
     * @brief Get the transform component of an entity
     *
     * @param e entity to get the transform component from
     * @return Components::LocalTransform* the transform component of the
     * entity, or nullptr if the entity does not have a transform component or
     * if an error occurs.
     */
    Components::LocalTransform *get_transform(Entity &e);

    /**
     * @brief Destroy an entity
     *
     * Removing it from the registry and freeing its resources.
     *
     * @param e entity to destroy
     */
    void destroy_entity(Entity &e);

    /**
     * @brief instantiate a prefab at a given position.
     *
     * @param prefabPath path to the prefab to instantiate, relative to the
     * Assets/ directory.
     * @param position position to instantiate the prefab at, in world
     * coordinates.
     */
    std::optional<Entity> instantiate(const std::string &prefabPath,
                                      const glm::vec2 &position);

    // ------------ Input API ------------
    /**
     * @brief check if a specific key is currently pressed.
     *
     * @param key the name of the key to check (e.g., "W", "A", "S", "D",
     * "Space", etc.)
     * @return true key is currently pressed
     * @return false key is not currently pressed or unknown key name provided
     */
    bool is_key_pressed(const std::string &key);
    /**
     * @brief check if a specific key is currently released.
     *
     * @param key the name of the key to check (e.g., "W", "A", "S", "D",
     * "Space", etc.)
     * @return true key is currently released
     * @return false key is not currently released or unknown key name provided
     */
    bool is_key_released(const std::string &key);
    /**
     * @brief check if a specific key is currently held down.
     *
     * @param key the name of the key to check (e.g., "W", "A", "S", "D",
     * "Space", etc.)
     * @return true key is currently held down
     * @return false key is not currently held down or unknown key name provided
     */
    bool is_key_held(const std::string &key);

    // ----------- Scene API ------------
    /**
     * @brief load a scene by its name.
     *
     * @param sceneName name of the scene to load
     */
    void load_scene(const std::string &sceneName);
    /**
     * @brief unload a scene by its name.
     *
     * @param sceneName name of the scene to unload
     */
    void unload_scene(const std::string &sceneName);

  private:
    sol::state &m_lua;
    entt::registry &m_registry;
    Input *m_input = nullptr;
    SceneManager *m_sceneManager = nullptr;
};

} // namespace Hylozoa

#endif /* !SCRIPTING_API_HPP_ */