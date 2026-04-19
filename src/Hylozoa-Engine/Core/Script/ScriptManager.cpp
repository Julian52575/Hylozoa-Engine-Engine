/*
** EPITECH PROJECT, 2026
** Hylozoa Engine
** File description:
** Script Manager [source file]
*/

#include "ScriptManager.hpp"
#include "ScriptingAPI.hpp"

namespace Hylozoa {

ScriptManager::ScriptManager()
{
}

ScriptManager::~ScriptManager()
{
    m_lua.collect_garbage();
}

void ScriptManager::initialize()
{
    m_lua.open_libraries(sol::lib::base, sol::lib::string, sol::lib::math, sol::lib::table);

    registerTypes();

    exposeAPI();
}

void ScriptManager::exposeAPI()
{
    m_lua.set_function("yolo", Scripting::API::yolo);
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

    m_lua.new_usertype<Entity>("Entity",
        "get_transform", &Entity::getTransform
    );
}


void ScriptManager::createScriptComponent(Components::Script &scriptComponent, const std::string &script)
{
    scriptComponent.env = sol::environment(m_lua, sol::create, m_lua.globals());

    m_lua.script(script, scriptComponent.env);

    scriptComponent.onUpdate = scriptComponent.env["onUpdate"];
}

}