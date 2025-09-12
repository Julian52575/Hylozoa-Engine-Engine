/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Movements System (header)
*/

#ifndef MOVEMENTS_HPP_
#define MOVEMENTS_HPP_

#include <flecs.h>
#include "Hylozoa-Engine/Components/Transform.hpp"

namespace Hylozoa {

inline void registerMovementsSystem(flecs::world &world) {
    world.system<Transform2D>()
        .each([](Transform2D &t2d) {
            t2d.x += 1.0f;
            t2d.y += 0.5f;
        });
}

} // namespace Hylozoa


#endif