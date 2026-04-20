/*
** EPITECH PROJECT, 2026
** Hylozoa Engine
** File description:
** Scripting API [header]
*/

#ifndef SCRIPTING_API_HPP_
#define SCRIPTING_API_HPP_

#include "Hylozoa-Engine/Core/Entities/Entity.hpp"

namespace Hylozoa::Scripting::API {

// whatever free function to expose or set engine function to a lua environment.
void yolo();

void log_message(const std::string& message);

// ------------ Entity API ------------
Components::LocalTransform *get_transform(Entity& e);

}

#endif /* !SCRIPTING_API_HPP_ */