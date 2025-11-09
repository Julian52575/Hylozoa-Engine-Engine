/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Collisions System [Source]
*/

#include "Hylozoa-Engine/Components/Physics/Physics.hpp"
#include "Hylozoa-Engine/Components/Transform/Transform.hpp"

#include "Collision.hpp"

#include <entt/entt.hpp>

namespace Hylozoa {

void CollisionSystem::createBodies() {
  auto view = _registry->view<Components::RigidBodyComponent, WorldTransform>();

  for (auto entity : view) {
    auto &rigidbody = view.get<Components::RigidBodyComponent>(entity);
    if (B2_IS_NON_NULL(rigidbody.bodyId)) {
      continue; // Body already created
    }
    auto &transform = view.get<WorldTransform>(entity);

    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = rigidbody.type;
    bodyDef.position.x = pixelsToMeters(transform.position.x);
    bodyDef.position.y = pixelsToMeters(transform.position.y);
    bodyDef.rotation = b2MakeRot(transform.rotation);
    bodyDef.gravityScale = rigidbody.gravityScale;
    bodyDef.linearDamping = rigidbody.linearDamping;
    bodyDef.angularDamping = rigidbody.angularDamping;
    bodyDef.fixedRotation = rigidbody.fixedRotation;
    bodyDef.isBullet = rigidbody.isBullet;
    bodyDef.isAwake = rigidbody.isAwake;
    bodyDef.isEnabled = rigidbody.isEnabled;

    // Store entity in user data
    bodyDef.userData = reinterpret_cast<void *>(static_cast<uint32_t>(entity));

    rigidbody.bodyId = b2CreateBody(m_world, &bodyDef);
    Name nameBody = _registry->get<Name>(entity);
    std::cout << "[CollisionSystem] Created body for entity " << nameBody.name
              << "\n";
  }
}

static void createBoxColliders(entt::registry &r) {
  auto boxView =
      r.view<Components::RigidBodyComponent, Components::ColliderComponent,
             Components::BoxColliderComponent>();

  for (auto entity : boxView) {
    auto &rb = boxView.get<Components::RigidBodyComponent>(entity);
    if (B2_IS_NULL(rb.bodyId)) {
      continue; // Body not initialized yet
    }

    auto &collider = boxView.get<Components::ColliderComponent>(entity);
    if (B2_IS_NON_NULL(collider.shapeId)) {
      continue; // Collider already created
    }
    auto &box = boxView.get<Components::BoxColliderComponent>(entity);

    b2Polygon polygon = b2MakeBox(pixelsToMeters(box.width * 0.5f),
                                  pixelsToMeters(box.height * 0.5f));

    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = collider.density;
    shapeDef.material.friction = collider.friction;
    shapeDef.material.restitution = collider.restitution;
    shapeDef.material.rollingResistance = collider.rollingResistance;
    shapeDef.material.tangentSpeed = collider.tangentSpeed;
    shapeDef.filter = collider.filter;
    shapeDef.isSensor = collider.isSensor;

    shapeDef.userData = reinterpret_cast<void *>(static_cast<uint32_t>(entity));
    collider.shapeId = b2CreatePolygonShape(rb.bodyId, &shapeDef, &polygon);
    Name nameBody = r.get<Name>(entity);
    std::cout << "[CollisionSystem] Created box collider for entity "
              << nameBody.name << "\n";
  }
}

static void createCircleColliders(entt::registry &r) {
  auto circleView =
      r.view<Components::RigidBodyComponent, Components::ColliderComponent,
             Components::CircleColliderComponent>();

  for (auto entity : circleView) {
    auto &rb = circleView.get<Components::RigidBodyComponent>(entity);
    if (B2_IS_NULL(rb.bodyId)) {
      continue; // Body not initialized yet
    }

    auto &collider = circleView.get<Components::ColliderComponent>(entity);
    if (B2_IS_NON_NULL(collider.shapeId)) {
      continue; // Collider already created
    }
    auto &circle = circleView.get<Components::CircleColliderComponent>(entity);

    b2Circle circleShape;
    circleShape.center = circle.offset;
    circleShape.radius = circle.radius;

    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = collider.density;
    shapeDef.material.friction = collider.friction;
    shapeDef.material.restitution = collider.restitution;
    shapeDef.material.rollingResistance = collider.rollingResistance;
    shapeDef.filter = collider.filter;
    shapeDef.isSensor = collider.isSensor;

    shapeDef.userData = reinterpret_cast<void *>(static_cast<uint32_t>(entity));
    collider.shapeId = b2CreateCircleShape(rb.bodyId, &shapeDef, &circleShape);
    Name nameBody = r.get<Name>(entity);
    std::cout << "[CollisionSystem] Created circle collider for entity "
              << nameBody.name << "\n";
  }
}

static void createCapsuleColliders(entt::registry &r) {
  auto capsuleView =
      r.view<Components::RigidBodyComponent, Components::ColliderComponent,
             Components::CapsuleColliderComponent>();

  for (auto entity : capsuleView) {
    auto &rb = capsuleView.get<Components::RigidBodyComponent>(entity);
    if (B2_IS_NULL(rb.bodyId)) {
      continue; // Body not initialized yet
    }

    auto &collider = capsuleView.get<Components::ColliderComponent>(entity);
    if (B2_IS_NON_NULL(collider.shapeId)) {
      continue; // Collider already created
    }
    auto &capsule =
        capsuleView.get<Components::CapsuleColliderComponent>(entity);

    b2Capsule capsuleShape;
    capsuleShape.center1 = capsule.center1;
    capsuleShape.center2 = capsule.center2;
    capsuleShape.radius = capsule.radius;

    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = collider.density;
    shapeDef.material.friction = collider.friction;
    shapeDef.material.restitution = collider.restitution;
    shapeDef.material.rollingResistance = collider.rollingResistance;
    shapeDef.filter = collider.filter;
    shapeDef.isSensor = collider.isSensor;

    shapeDef.userData = reinterpret_cast<void *>(static_cast<uint32_t>(entity));
    collider.shapeId =
        b2CreateCapsuleShape(rb.bodyId, &shapeDef, &capsuleShape);
    Name nameBody = r.get<Name>(entity);
    std::cout << "[CollisionSystem] Created capsule collider for entity "
              << nameBody.name << "\n";
  }
}

void CollisionSystem::createColliders() {
  // Box colliders
  createBoxColliders(*_registry);

  // Circle colliders
  createCircleColliders(*_registry);

  // Capsule colliders
  createCapsuleColliders(*_registry);
}

// Sync Box2D transforms back to ECS
void CollisionSystem::syncTransforms() {
  auto view = _registry->view<WorldTransform, Components::RigidBodyComponent>();
  for (auto entity : view) {
    auto &transform = view.get<WorldTransform>(entity);
    auto &rb = view.get<Components::RigidBodyComponent>(entity);

    if (b2Body_IsValid(rb.bodyId)) {
      auto pos = b2Body_GetPosition(rb.bodyId);
      transform.position.x = metersToPixels(pos.x);
      transform.position.y = metersToPixels(pos.y);
      transform.rotation = b2Rot_GetAngle(b2Body_GetRotation(rb.bodyId));

      Name nameBody = _registry->get<Name>(entity);
      std::cout << "[CollisionSystem] Entity : " << nameBody.name
                << " synced to position (" << transform.position.x << ", "
                << transform.position.y << ") and rotation ("
                << transform.rotation << " radians)\n";

      // Optional: sync velocities
      // if (registry.all_of<VelocityComponent>(entity)) {
      //     auto& vel = registry.get<VelocityComponent>(entity);
      //     vel.linear = b2Body_GetLinearVelocity(rb.bodyId);
      //     vel.angular = b2Body_GetAngularVelocity(rb.bodyId);
      // }
    }
  }
}

static void processContactBeginEvents(b2ContactEvents &ContactEvents,
                                      entt::registry &registry) {
  std::cout << "[CollisionSystem] Processing " << ContactEvents.beginCount
            << " begin contact events.\n";

  // Process begin contact
  for (int i = 0; ContactEvents.beginCount; ++i) {
    auto &event = ContactEvents.beginEvents[i];
    // Convert userData back to entity
    auto entityA = static_cast<entt::entity>(
        reinterpret_cast<uintptr_t>(b2Shape_GetUserData(event.shapeIdA)));
    auto entityB = static_cast<entt::entity>(
        reinterpret_cast<uintptr_t>(b2Shape_GetUserData(event.shapeIdB)));

    Name nameA = registry.get<Name>(entityA);
    Name nameB = registry.get<Name>(entityB);

    // Debug for now
    std::cout << "[CollisionSystem] Begin Contact between " << nameA.name
              << " and " << nameB.name << std::endl;
  }
}

static void processContactHitEvents(b2ContactEvents &ContactEvents,
                                    entt::registry &registry) {
  std::cout << "[CollisionSystem] Processing " << ContactEvents.hitCount
            << " hit contact events.\n";

  // Process hit contact
  for (int i = 0; ContactEvents.hitCount; ++i) {
    auto &event = ContactEvents.hitEvents[i];
    // Convert userData back to entity
    auto entityA = static_cast<entt::entity>(
        reinterpret_cast<uintptr_t>(b2Shape_GetUserData(event.shapeIdA)));
    auto entityB = static_cast<entt::entity>(
        reinterpret_cast<uintptr_t>(b2Shape_GetUserData(event.shapeIdB)));

    Name nameA = registry.get<Name>(entityA);
    Name nameB = registry.get<Name>(entityB);

    // Debug for now
    std::cout << "[CollisionSystem] Hit Contact between " << nameA.name
              << " and " << nameB.name << std::endl;
  }
}

static void processContactEndEvents(b2ContactEvents &ContactEvents,
                                    entt::registry &registry) {
  std::cout << "[CollisionSystem] Processing " << ContactEvents.endCount
            << " end contact events.\n";

  // Process end contact
  for (int i = 0; ContactEvents.endCount; ++i) {
    auto &event = ContactEvents.endEvents[i];
    // Convert userData back to entity
    auto entityA = static_cast<entt::entity>(
        reinterpret_cast<uintptr_t>(b2Shape_GetUserData(event.shapeIdA)));
    auto entityB = static_cast<entt::entity>(
        reinterpret_cast<uintptr_t>(b2Shape_GetUserData(event.shapeIdB)));

    Name nameA = registry.get<Name>(entityA);
    Name nameB = registry.get<Name>(entityB);

    // Debug for now
    std::cout << "[CollisionSystem] End Contact between " << nameA.name
              << " and " << nameB.name << std::endl;
  }
}

static void processSensorBeginEvents(b2SensorEvents &sensorEvents,
                                     entt::registry &registry) {
  std::cout << "[CollisionSystem] Processing " << sensorEvents.beginCount
            << " begin sensor events.\n";

  // Process sensor begin touch
  for (int i = 0; i < sensorEvents.beginCount; ++i) {
    auto &event = sensorEvents.beginEvents[i];
    // Convert userData back to entity
    auto sensorEntity = static_cast<entt::entity>(
        reinterpret_cast<uintptr_t>(b2Shape_GetUserData(event.sensorShapeId)));
    auto otherEntity = static_cast<entt::entity>(
        reinterpret_cast<uintptr_t>(b2Shape_GetUserData(event.visitorShapeId)));

    Name sensorEntityName = registry.get<Name>(sensorEntity);
    Name otherEntityName = registry.get<Name>(otherEntity);

    // Debug for now
    std::cout << "[CollisionSystem] Sensor Begin Touch between "
              << sensorEntityName.name << " and " << otherEntityName.name
              << std::endl;
  }
}

static void processSensorEndEvents(b2SensorEvents &sensorEvents,
                                   entt::registry &registry) {
  std::cout << "[CollisionSystem] Processing " << sensorEvents.endCount
            << " end sensor events.\n";
  // Process sensor end touch
  for (int i = 0; i < sensorEvents.endCount; ++i) {
    auto &event = sensorEvents.endEvents[i];
    // Convert userData back to entity
    auto sensorEntity = static_cast<entt::entity>(
        reinterpret_cast<uintptr_t>(b2Shape_GetUserData(event.sensorShapeId)));
    auto otherEntity = static_cast<entt::entity>(
        reinterpret_cast<uintptr_t>(b2Shape_GetUserData(event.visitorShapeId)));

    Name sensorEntityName = registry.get<Name>(sensorEntity);
    Name otherEntityName = registry.get<Name>(otherEntity);
    // Debug for now
    std::cout << "[CollisionSystem] Sensor End Touch between "
              << sensorEntityName.name << " and " << otherEntityName.name
              << std::endl;
  }
}

// Process Box2D events
void CollisionSystem::processEvents() {
  // Contact events
  b2ContactEvents ContactEvents = b2World_GetContactEvents(m_world);

  processContactBeginEvents(ContactEvents, *_registry);
  processContactHitEvents(ContactEvents, *_registry);
  processContactEndEvents(ContactEvents, *_registry);

  // Sensor events
  b2SensorEvents sensorEvents = b2World_GetSensorEvents(m_world);

  processSensorBeginEvents(sensorEvents, *_registry);
  processSensorEndEvents(sensorEvents, *_registry);
}

} // namespace Hylozoa