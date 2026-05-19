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
    m_lua.set_function("get_name", &ScriptingAPI::get_name, this);
    m_lua.set_function("destroy_entity", &ScriptingAPI::destroy_entity, this);
    m_lua.set_function("instantiate", &ScriptingAPI::instantiate, this);

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

// ---------------------Utility API---------------------
void ScriptingAPI::yolo() {
    std::cout << "YOLO from the hylozoa scripting API" << std::endl;
}

void ScriptingAPI::log_message(sol::variadic_args va)
{
    std::ostringstream oss;

    bool first = true;
    static sol::protected_function tostring_func = m_lua["tostring"];

    for (auto v : va) {
        if (!first)
            oss << " ";

        sol::object obj = v.get<sol::object>();

        switch (obj.get_type()) {
            case sol::type::number:
                oss << obj.as<double>();
                break;

            case sol::type::string:
                oss << obj.as<std::string>();
                break;

            case sol::type::boolean:
                oss << (obj.as<bool>() ? "true" : "false");
                break;

            default:
                auto result = tostring_func(obj);
                if (result.valid()) {
                    oss << result.get<std::string>();
                } else {
                    oss << "<unsuported type>";
                }
                break;
        }

        first = false;
    }

    std::cout << "[Script-log] " << oss.str() << std::endl;
}


// ---------------------Entity API---------------------
Components::LocalTransform* ScriptingAPI::get_transform(Entity& e) {
    try {
        auto& comp = e.getComponent<Components::LocalTransform>();
        return &comp;
    } catch (const std::exception& ex) {
        if (Hylozoa::Settings::getInstance().getSettings().verbose) {
            std::cout << "[Script-API] Error in get_transform: " << ex.what() << std::endl;
        }
        return nullptr;
    }
}

const std::string& ScriptingAPI::get_name(Entity& e) {
    try {
        auto& comp = e.getComponent<Components::Name>();
        return comp.name;
    } catch (const std::exception& ex) {
        if (Hylozoa::Settings::getInstance().getSettings().verbose) {
            std::cout << "[Script-API] Error in get_name: " << ex.what() << std::endl;
        }
        static std::string emptyString = "";
        return emptyString;
    }
}

void ScriptingAPI::destroy_entity(Entity& e) {
    try {
        e.addTagComponent<Components::HylozoaInternal::PendingDestruction>();
    } catch (const std::exception& ex) {
        if (Hylozoa::Settings::getInstance().getSettings().verbose) {
            std::cout << "[Script-API] Error in destroy_entity: " << ex.what() << std::endl;
        }
    }
}

std::optional<Entity> ScriptingAPI::instantiate(const std::string& prefabPath, const glm::vec2& position) {
    try {
        if (!m_sceneManager) {
            if (Hylozoa::Settings::getInstance().getSettings().verbose) {
                std::cout << "[Script-API] Warning: SceneManager not initialized. Cannot instantiate prefab.\n";
            }
            return std::nullopt;
        }
        std::cout << "[Script-API] Instantiating prefab '" << prefabPath << "' at position (" << position.x << ", " << position.y << ")\n";
        return m_sceneManager->instantiatePrefab(prefabPath, position);
    } catch (const std::runtime_error& ex) {
        if (Hylozoa::Settings::getInstance().getSettings().verbose) {
            std::cout << "[Script-API] Runtime error in instantiate: " << ex.what() << std::endl;
        }
        return std::nullopt;
    } catch (const std::exception& ex) {
        if (Hylozoa::Settings::getInstance().getSettings().verbose) {
            std::cout << "[Script-API] Unknown error in instantiate: " << ex.what() << std::endl;
        }
        return std::nullopt;
    }
    return std::nullopt;
}

// ---------------------Input API---------------------
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


// ---------------------Scene API---------------------
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