/*
** EPITECH PROJECT, 2026
** Hylozoa Engine
** File description:
** Scripting API [source]
*/

#include "ScriptingAPI.hpp"
#include "Hylozoa-Engine/Core/Settings.hpp"
#include "Hylozoa-Engine/Core/IO/Input.hpp"

#include <iostream>

namespace Hylozoa {

void ScriptingAPI::yolo() {
    std::cout << "YOLO from the hylozoa scripting API" << std::endl;
}

void ScriptingAPI::log_message(sol::variadic_args va) {
    std::stringstream ss;
    sol::function tostring = m_lua["tostring"];

    for (size_t i = 0; i < va.size(); ++i) {
        if (i > 0) ss << "\t";
        ss << tostring(va[i].get<sol::object>()).get<std::string>();
    }

    std::cout << "[Script-log] " << ss.str() << std::endl;
}

Components::LocalTransform* ScriptingAPI::get_transform(Entity& e) {
    try {
        return &e.getComponent<Components::LocalTransform>();
    } catch (const std::exception& ex) {
        if (Hylozoa::Settings::getInstance().getSettings().verbose) {
            std::cout << "[Script-API] Error in get_transform: " << ex.what() << std::endl;
        }
        return nullptr;
    }
}


bool ScriptingAPI::is_key_pressed(const std::string& key) {
    auto& input = m_registry.ctx().get<Hylozoa::Input>();

    return input.isKeyDown(key);
}

bool ScriptingAPI::is_key_released(std::string_view key) {
    auto& input = m_registry.ctx().get<Hylozoa::Input>();

    return input.isKeyUp(key);
}

bool ScriptingAPI::is_key_held(std::string_view key) {
    auto& input = m_registry.ctx().get<Hylozoa::Input>();

    return input.isKeyHeld(key);
}

}