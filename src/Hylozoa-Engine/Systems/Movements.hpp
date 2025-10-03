/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Movements System (header)
*/

#ifndef MOVEMENTS_HPP_
#define MOVEMENTS_HPP_

#include <flecs.h>
#include <iostream>
#include "Hylozoa-Engine/Components/Transform.hpp"

namespace Hylozoa {

inline void registerMovementsSystem(flecs::world &world) {
    world.system<Transform2D>()
        .each([](Transform2D &t2d) {
            t2d.x += 1.0f;
            t2d.y += 0.5f;
        });
}

inline void registerLocalToWorldSystem(flecs::world &world) {
    world.system<LocalTransform>("LocalToWorld")
        .kind(flecs::OnUpdate)
        .cascade(flecs::ChildOf) // conmentaire pour tester sans cascade (parse sans trier les child par rapport aux enfants)
        .each([](flecs::entity e, LocalTransform &localTransform) {
            std::cout << "name: " << e.name() << " x pos = " << localTransform.position.x << std::endl;
            matrix3x3 localMat = matrix3x3::fromTransform(e.get<LocalTransform>());
            
            if (auto parent = e.target(flecs::ChildOf)) {
                if (parent.has<GlobalTransform>()) {
                    GlobalTransform parentGlobal = parent.get<GlobalTransform>();
                    matrix3x3 parentMat = matrix3x3::fromTransform(parentGlobal);
                    std::cout << localMat[0][2] << " " << localMat[1][2] << std::endl;
                    matrix3x3 worldMatrix = parentMat * localMat;
                    e.set<GlobalTransform>(worldMatrix.toGlobalTransform());
                } else {
                    std::cout << "Parent has no GlobalTransform" << std::endl;
                    e.set<GlobalTransform>(localMat.toGlobalTransform());
                }
            } else {
                // No parent, root entity uses local as global
                std::cout << "No parent for entity: " << e.name() << std::endl;
                e.set<GlobalTransform>(localMat.toGlobalTransform());
            }
        });
}

} // namespace Hylozoa


#endif