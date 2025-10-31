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

struct RigidBodyComponent {
  b2BodyId bodyId{b2_nullBodyId};
  b2BodyType type{b2_dynamicBody}; // dynamic, static, or kinematic

  // Body properties
  float gravityScale{1.0f};
  float linearDamping{0.0f};
  float angularDamping{0.0f};
  bool fixedRotation{false};
  bool isBullet{false}; // For continuous collision detection
  bool isAwake{true};
  bool isEnabled{true};
};

struct ColliderComponent {
  b2ShapeId shapeId{b2_nullShapeId};

  float density{1.0f};
  float friction{0.3f};
  float restitution{0.0f}; // Bounciness

  float rollingResistance{0.0f}; // For circles/capsules
  float tangentSpeed{0.0f};      // Conveyor belt effect

  b2Filter filter{};
  bool isSensor{false};
};

struct BoxColliderComponent {
  float width{1.0f};
  float height{1.0f};
  b2Vec2 offset{0.0f, 0.0f};
};

struct CircleColliderComponent {
  float radius{0.5f};
  b2Vec2 offset{0.0f, 0.0f};
};

struct CapsuleColliderComponent {
  b2Vec2 center1{0.0f, -0.5f};
  b2Vec2 center2{0.0f, 0.5f};
  float radius{0.5f};
};

} // namespace Components
} // namespace Hylozoa

#endif /* !PHYSICS_HPP_ */