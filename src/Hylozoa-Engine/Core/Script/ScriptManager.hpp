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

    /**
     * @brief Initialize the necessary for the scripting engine
     * 
     * This initialize the Lua state, opens the standard libraries, registers the engine's API and types to Lua, and sets up any necessary API bidings.
     */
    void initialize();

    /**
     * @brief Generate the content of a Script component based on a given script file or raw script content.
     * 
     * @param scriptComponent the script component to populate with the generated content.
     * @param script the path to the script file or the raw script content, depending on the value of isRaw.
     * @param isRaw a boolean flag indicating whether the script parameter is raw Lua code (true) or a file path to a Lua script (false). Defaults to true.
     */
    void createScriptComponent(Components::Script &scriptComponent, const std::string &script, bool isRaw = true);

private:
    sol::state m_lua;
private:
    void exposeAPI();
    void registerTypes();
};


}

#endif /* !SCRIPT_MANAGER_HPP_ */