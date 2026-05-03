/*
** EPITECH PROJECT, 2026
** Hylozoa Engine
** File description:
** Scripting API [source]
*/

#include "ScriptingAPI.hpp"
#include "Hylozoa-Engine/Core/Settings.hpp"
#include "Hylozoa-Engine/Core/IO/Input.hpp"
#include "Hylozoa-Engine/Core/Scenes/Scene.hpp"

#include <iostream>

namespace Hylozoa {

ScriptingAPI::ScriptingAPI(entt::registry& registry, sol::state& lua) : m_registry(registry), m_lua(lua) {

    m_lua.set_function("yolo", &ScriptingAPI::yolo, this);

    // ---------------------Utility API---------------------
    m_lua.set_function("log_message", &ScriptingAPI::log_message, this);
    m_lua.set_function("print", &ScriptingAPI::log_message, this);

    // ---------------------Entity API---------------------
    m_lua.set_function("get_transform", &ScriptingAPI::get_transform, this);

    // ---------------------Input API---------------------
    m_lua.set_function("is_key_pressed", &ScriptingAPI::is_key_pressed, this);
    m_lua.set_function("is_key_released", &ScriptingAPI::is_key_released, this);
    m_lua.set_function("is_key_held", &ScriptingAPI::is_key_held, this);

    // ---------------------Scene API---------------------
    m_lua.set_function("load_scene", &ScriptingAPI::load_scene, this);
    m_lua.set_function("unload_scene", &ScriptingAPI::unload_scene, this);
}

void ScriptingAPI::initialize() {
    m_input = &m_registry.ctx().get<Hylozoa::Input>();
    m_sceneManager = &m_registry.ctx().get<Hylozoa::SceneManager>();
}

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
    if (!m_input) {
        if (Hylozoa::Settings::getInstance().getSettings().verbose) {
            std::cout << "[Script-API] Warning: Input system not initialized. Cannot check key state.\n";
        }
        return false;
    }
    return m_input->isKeyDown(key);
}

bool ScriptingAPI::is_key_released(const std::string& key) {
    if (!m_input) {
        if (Hylozoa::Settings::getInstance().getSettings().verbose) {
            std::cout << "[Script-API] Warning: Input system not initialized. Cannot check key state.\n";
        }
        return false;
    }
    return m_input->isKeyUp(key);
}

bool ScriptingAPI::is_key_held(const std::string& key) {
    if (!m_input) {
        if (Hylozoa::Settings::getInstance().getSettings().verbose) {
            std::cout << "[Script-API] Warning: Input system not initialized. Cannot check key state.\n";
        }
        return false;
    }
    return m_input->isKeyHeld(key);
}

void ScriptingAPI::load_scene(const std::string& sceneName) {
    try {
        if (!m_sceneManager) {
            if (Hylozoa::Settings::getInstance().getSettings().verbose) {
                std::cout << "[Script-API] Warning: SceneManager not initialized. Cannot load scene.\n";
            }
            return;
        }
        m_sceneManager->loadScene(sceneName);
    } catch (const std::runtime_error& ex) {
        if (Hylozoa::Settings::getInstance().getSettings().verbose) {
            std::cout << "[Script-API] Runtime error in load_scene: " << ex.what() << std::endl;
        }
    } catch (const std::exception& ex) {
        if (Hylozoa::Settings::getInstance().getSettings().verbose) {
            std::cout << "[Script-API] Unknown error in load_scene: " << ex.what() << std::endl;
        }
    }
}

void ScriptingAPI::unload_scene(const std::string& sceneName) {
    try {
        if (!m_sceneManager) {
            if (Hylozoa::Settings::getInstance().getSettings().verbose) {
                std::cout << "[Script-API] Warning: SceneManager not initialized. Cannot unload scene.\n";
            }
            return;
        }
        m_sceneManager->unloadScene(sceneName);
    } catch (const std::runtime_error& ex) {
        if (Hylozoa::Settings::getInstance().getSettings().verbose) {
            std::cout << "[Script-API] Runtime error in unload_scene: " << ex.what() << std::endl;
        }
    } catch (const std::exception& ex) {
        if (Hylozoa::Settings::getInstance().getSettings().verbose) {
            std::cout << "[Script-API] Unknown error in unload_scene: " << ex.what() << std::endl;
        }
    }
}

}