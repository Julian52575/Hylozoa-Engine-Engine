/*
** EPITECH PROJECT, 2026
** Hylozoa Engine
** File description:
** Script Manager [source file]
*/

#include "ScriptManager.hpp"
#include "ScriptingAPI.hpp"
#include "Hylozoa-Engine/Core/Settings.hpp"

namespace Hylozoa {

static std::chrono::steady_clock::time_point script_start_time;
static const double MAX_SCRIPT_TIME_SECONDS = 0.5;

ScriptManager::~ScriptManager()
{
    m_lua.collect_garbage();
}

void ScriptManager::initialize()
{
    m_lua.open_libraries(sol::lib::base, sol::lib::string, sol::lib::math, sol::lib::table);

    registerTypes();
    m_api.initialize();
}

void ScriptManager::registerTypes()
{
    m_lua.new_usertype<glm::vec2>("Vec2",
        "x", &glm::vec2::x,
        "y", &glm::vec2::y
    );

    m_lua.new_usertype<Components::LocalTransform>("Transform",
        "position", &Components::LocalTransform::position,
        "rotation", &Components::LocalTransform::rotation,
        "scale", &Components::LocalTransform::scale
    );

    m_lua.new_usertype<Entity>("Entity");

    m_lua.new_usertype<Components::HylozoaInternal::NoiseInfo>("NoiseInfo",
        "noiseName", &Components::HylozoaInternal::NoiseInfo::noiseName,
        "position", &Components::HylozoaInternal::NoiseInfo::position,
        "direction", &Components::HylozoaInternal::NoiseInfo::direction
    );
}

void watchdogHook(lua_State* L, lua_Debug* ar) {
    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed = now - script_start_time;

    if (elapsed.count() > MAX_SCRIPT_TIME_SECONDS) {
        luaL_error(L, "Script execution exceeded time limit");
    }
}

void ScriptManager::createScriptComponent(Components::Script &scriptComponent, const std::string &script, bool isRaw)
{
    scriptComponent.env = sol::environment(m_lua, sol::create, m_lua.globals());

    const std::string path = std::string(SDL_GetBasePath()) + std::string("Assets/") + script;

    script_start_time = std::chrono::steady_clock::now();
    lua_sethook(m_lua, watchdogHook, LUA_MASKCOUNT, 1000);

    try {
        if (isRaw) {
            auto result = m_lua.safe_script(script, scriptComponent.env);

            if (!result.valid()) {
                sol::error err = result;

                if (Hylozoa::Settings::getInstance().getSettings().verbose) {
                    std::cerr << "Error loading script content: " << err.what() << std::endl;
                }

                return;
            }
        } else {
            auto result = m_lua.safe_script_file(path, scriptComponent.env);

            if (!result.valid()) {
                sol::error err = result;

                if (Hylozoa::Settings::getInstance().getSettings().verbose) {
                    std::cerr << "Error loading script content: " << err.what() << std::endl;
                }

                return;
            }
        }
    } catch (const std::exception &e) {
        lua_sethook(m_lua, nullptr, 0, 0);
        return;
    }
    lua_sethook(m_lua, nullptr, 0, 0);

    scriptComponent.onUpdate = scriptComponent.env["onUpdate"];
    scriptComponent.onNoise = scriptComponent.env["onNoise"];
}

}