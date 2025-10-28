/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Circle collider Component [header]
*/

#ifndef PHYSICS_HPP_
#define PHYSICS_HPP_

#include <box2d/box2d.h>

namespace Hylozoa {
    namespace Components {
        using PhysicsBodyConfig = b2BodyDef;
        using PhysicsBody = b2BodyId;

        using ShapeColliderConfig = b2ShapeDef;
        using ShapeCollider = b2Polygon;

    } // namespace Components
}


#endif /* !PHYSICS_HPP_ */