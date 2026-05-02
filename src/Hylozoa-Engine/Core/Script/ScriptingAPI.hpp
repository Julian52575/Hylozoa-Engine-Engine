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

class ScriptingAPI
{
public:
    ScriptingAPI(entt::registry& registry, sol::state& lua);
    ~ScriptingAPI() = default;

    void initialize();

    void yolo();

    void log_message(sol::variadic_args va);

    // ------------ Entity API ------------
    Components::LocalTransform *get_transform(Entity& e);


    // ------------ Input API ------------
    bool is_key_pressed(const std::string& key);
    bool is_key_released(const std::string& key);
    bool is_key_held(const std::string& key);

    // ----------- Scene API ------------
    void load_scene(const std::string& sceneName);
    void unload_scene(const std::string& sceneName);
private:
    sol::state& m_lua;
    entt::registry& m_registry;
    Input *m_input = nullptr;
    SceneManager* m_sceneManager = nullptr;
};

}

#endif /* !SCRIPTING_API_HPP_ */