/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Physics Components [header]
*/

#ifndef PHYSICS_HPP_
#define PHYSICS_HPP_

#include <box2d/box2d.h>

constexpr float PIXELS_PER_METER = 50.0f;

inline float metersToPixels(float meters) {
  return meters * PIXELS_PER_METER;
}
inline float pixelsToMeters(float pixels) {
  return pixels / PIXELS_PER_METER;
}

namespace Hylozoa {
/**
 * @namespace Components
 * @brief Contains various components related to physics simulation in the Hylozoa Engine.
 */
namespace Components {

  /**
 * @struct RigidBodyComponent
 * @brief Represents a rigid body in the physics simulation.
 * 
 * This component defines the properties of a rigid body, such as its type, damping, 
 * and collision detection settings.
 * 
 * @var bodyId
 *      The unique identifier for the body in the physics world.
 * @var type
 *      The type of the body (dynamic, static, or kinematic).static by default.
 * @var gravityScale
 *      The scale factor for gravity applied to the body.
 * @var linearDamping
 *      The linear damping applied to the body to reduce velocity over time.
 * @var angularDamping
 *      The angular damping applied to the body to reduce rotational velocity over time.
 * @var fixedRotation
 *      Whether the body is restricted from rotating.
 * @var isBullet
 *      Whether the body uses continuous collision detection.
 * @var isAwake
 *      Whether the body is awake and active in the simulation.
 * @var isEnabled
 *      Whether the body is enabled in the simulation.
 */
struct RigidBodyComponent {
  b2BodyId bodyId{b2_nullBodyId};
  b2BodyType type{b2_staticBody}; // dynamic, static, or kinematic

  // Body properties
  float gravityScale{1.0f};
  float linearDamping{0.0f};
  float angularDamping{0.0f};
  bool fixedRotation{false};
  bool isBullet{false}; // For continuous collision detection
  bool isAwake{true};
  bool isEnabled{true};
};

/**
 * @struct ColliderComponent
 * @brief Represents a collider attached to a rigid body.
 * 
 * This component defines the properties of a collider, such as its shape, density, 
 * friction, and restitution.
 * 
 * @var shapeId
 *      The unique identifier for the shape in the physics world.
 * @var density
 *      The density of the collider, used to calculate mass.
 * @var friction
 *      The friction coefficient of the collider.
 * @var restitution
 *      The bounciness of the collider.
 * @var rollingResistance
 *      The rolling resistance for circular or capsule shapes.
 * @var tangentSpeed
 *      The tangent speed for conveyor belt effects.
 * @var filter
 *      The collision filter settings for the collider.
 * @var isSensor
 *      Whether the collider is a sensor (does not produce physical collisions).
 */
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

/**
 * @struct BoxColliderComponent
 * @brief Represents a box-shaped collider.
 * 
 * This component defines the dimensions and offset of a box collider.
 * 
 * @var width
 *      The width of the box collider.
 * @var height
 *      The height of the box collider.
 * @var offset
 *      The offset of the box collider relative to the rigid body.
 */
struct BoxColliderComponent {
  float width{1.0f};
  float height{1.0f};
  b2Vec2 offset{0.0f, 0.0f};
};

/**
 * @struct CircleColliderComponent
 * @brief Represents a circle-shaped collider.
 * 
 * This component defines the radius and offset of a circle collider.
 * 
 * @var radius
 *      The radius of the circle collider.
 * @var offset
 *      The offset of the circle collider relative to the rigid body.
 */
struct CircleColliderComponent {
  float radius{0.5f};
  b2Vec2 offset{0.0f, 0.0f};
};

/**
 * @struct CapsuleColliderComponent
 * @brief Represents a capsule-shaped collider.
 * 
 * This component defines the endpoints and radius of a capsule collider.
 * 
 * @var center1
 *      The first endpoint of the capsule's central line.
 * @var center2
 *      The second endpoint of the capsule's central line.
 * @var radius
 *      The radius of the capsule collider.
 */
struct CapsuleColliderComponent {
  b2Vec2 center1{0.0f, -0.5f};
  b2Vec2 center2{0.0f, 0.5f};
  float radius{0.5f};
};

} // namespace Components
} // namespace Hylozoa

#endif /* !PHYSICS_HPP_ */