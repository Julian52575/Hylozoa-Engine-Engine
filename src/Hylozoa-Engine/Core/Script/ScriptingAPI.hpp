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

namespace Hylozoa {

class ScriptingAPI
{
public:
    ScriptingAPI(entt::registry& registry, sol::state& lua) : m_registry(registry), m_lua(lua) {};
    ~ScriptingAPI() = default;

    void yolo();

    void log_message(sol::variadic_args va);

    // ------------ Entity API ------------
    Components::LocalTransform *get_transform(Entity& e);


    // ------------ Input API ------------
    bool is_key_pressed(const std::string& key);
    bool is_key_released(std::string_view key);
    bool is_key_held(std::string_view key);

private:
    sol::state& m_lua;
    entt::registry& m_registry;
};

}

#endif /* !SCRIPTING_API_HPP_ */