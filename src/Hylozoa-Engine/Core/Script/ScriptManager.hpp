/*
** EPITECH PROJECT, 2026
** Hylozoa Engine
** File description:
** Script Manager [header]
*/

#ifndef SCRIPT_MANAGER_HPP_
#define SCRIPT_MANAGER_HPP_

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>
#include "Hylozoa-Engine/Components/Components.hpp"

namespace Hylozoa {

class ScriptManager
{
public:
    ScriptManager();
    ~ScriptManager();

    void initialize();

    void createScriptComponent(Components::Script &scriptComponent, const std::string &script);

    sol::state& getLuaState() { return m_lua; }
private:
    sol::state m_lua;
private:
    void exposeAPI();
    void registerTypes();
};


}

#endif /* !SCRIPT_MANAGER_HPP_ */