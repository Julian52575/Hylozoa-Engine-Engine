/*
** EPITECH PROJECT, 2026
** Hylozoa Engine
** File description:
** Script Component [header]
*/

#ifndef SCRIPT_HPP_
#define SCRIPT_HPP_

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>
#include <string>
#include <unordered_map>

namespace Hylozoa::Components {
/**
 * @struct Script
 * @brief Component to store a script associated with an entity.
 *
 * This component holds an sol environment that can be used to execute Lua
 * scripts associated with an entity. It also contains a map of function names
 * to sol::function objects, allowing for easy access to specific functions
 * defined in the script.
 *
 * @var sol::environment env The sol environment for executing Lua scripts.
 * @var sol::function onUpdate A function that can be called every frame to
 * update the entity based on the script logic.
 * @var std::string scriptFile The file path of the Lua script associated with
 * this component.
 */
struct Script {
    sol::environment env;
    // std::unordered_map<std::string, sol::function> functions;
    sol::protected_function onUpdate;
    sol::protected_function onNoise;
    std::string scriptFile;
};
} // namespace Hylozoa::Components

#endif /* !SCRIPT_HPP_ */