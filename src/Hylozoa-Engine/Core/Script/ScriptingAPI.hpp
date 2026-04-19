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

Components::LocalTransform& get_transform(); // example of a function to get the transform of an entity, to be called from lua, will need to take an entity id as parameter and return a table with position, rotation and scale.

}

#endif /* !SCRIPTING_API_HPP_ */