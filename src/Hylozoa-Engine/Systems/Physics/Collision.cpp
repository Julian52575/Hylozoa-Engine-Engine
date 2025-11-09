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
#include <cstdint>
#include <iostream>

namespace Hylozoa {

void CollisionSystem::createBodies() {
    auto view = _registry->view<Components::RigidBodyComponent, WorldTransform>();

    for (auto entity : view) {
        auto& rigidbody = view.get<Components::RigidBodyComponent>(entity);
        if (B2_IS_NON_NULL(rigidbody.bodyId)) {
            continue; // Body already created
        }
        auto& transform = view.get<WorldTransform>(entity);

        b2BodyDef bodyDef = b2DefaultBodyDef();
        bodyDef.type = rigidbody.type;
        bodyDef.position.x = pixelsToMeters(transform.position.x);
        bodyDef.position.y = pixelsToMeters(transform.position.y);
        bodyDef.rotation = b2MakeRot(transform.rotation); // engine uses radians
        bodyDef.gravityScale = rigidbody.gravityScale;
        bodyDef.linearDamping = rigidbody.linearDamping;
        bodyDef.angularDamping = rigidbody.angularDamping;
        bodyDef.fixedRotation = rigidbody.fixedRotation;
        bodyDef.isBullet = rigidbody.isBullet;
        bodyDef.isAwake = rigidbody.isAwake;
        bodyDef.isEnabled = rigidbody.isEnabled;

        // Store entity in user data (use uintptr_t to avoid truncation)
        bodyDef.userData = reinterpret_cast<void*>(
            static_cast<uintptr_t>(entity)
        );

        rigidbody.bodyId = b2CreateBody(m_world, &bodyDef);
        if (_registry->all_of<Name>(entity)) {
            Name nameBody = _registry->get<Name>(entity);
            std::cout << "[CollisionSystem] Created body for entity "
                      << nameBody.name << "\n";
        }
    }
}

static void createBoxColliders(entt::registry &r) {
    auto boxView = r.view<Components::RigidBodyComponent, Components::ColliderComponent, Components::BoxColliderComponent>();

    for (auto entity : boxView) {
        auto& rb = boxView.get<Components::RigidBodyComponent>(entity);
        if (B2_IS_NULL(rb.bodyId)) {
            continue; // Body not initialized yet
        }

        auto& collider = boxView.get<Components::ColliderComponent>(entity);
        if (B2_IS_NON_NULL(collider.shapeId)) {
            continue; // Collider already created
        }
        auto& box = boxView.get<Components::BoxColliderComponent>(entity);

        // b2MakeBox expects half widths in meters
        b2Polygon polygon = b2MakeBox(
            pixelsToMeters(box.width * 0.5f), 
            pixelsToMeters(box.height * 0.5f)
        );

        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.density = collider.density;
        shapeDef.material.friction = collider.friction;
        shapeDef.material.restitution = collider.restitution;
        shapeDef.material.rollingResistance = collider.rollingResistance;
        shapeDef.material.tangentSpeed = collider.tangentSpeed;
        shapeDef.filter = collider.filter;
        shapeDef.isSensor = collider.isSensor;

        // Attach user data (entity)
        shapeDef.userData = reinterpret_cast<void*>(
            static_cast<uintptr_t>(entity)
        );
        collider.shapeId = b2CreatePolygonShape(
            rb.bodyId, &shapeDef, &polygon
        );
        if (r.all_of<Name>(entity)) {
            Name nameBody = r.get<Name>(entity);
            std::cout << "[CollisionSystem] Created box collider for entity " 
                      << nameBody.name << "\n";
        }
    }
}

static void createCircleColliders(entt::registry &r) {
    auto circleView = r.view<Components::RigidBodyComponent, Components::ColliderComponent, Components::CircleColliderComponent>();

    for (auto entity : circleView) {
        auto& rb = circleView.get<Components::RigidBodyComponent>(entity);
        if (B2_IS_NULL(rb.bodyId)) {
            continue; // Body not initialized yet
        }

        auto& collider = circleView.get<Components::ColliderComponent>(entity);
        if (B2_IS_NON_NULL(collider.shapeId)) {
            continue; // Collider already created
        }
        auto& circle = circleView.get<Components::CircleColliderComponent>(entity);

        b2Circle circleShape;
        // Convert offset + radius from pixels -> meters
        circleShape.center = b2Vec2{
            pixelsToMeters(circle.offset.x),
            pixelsToMeters(circle.offset.y)
        };
        circleShape.radius = pixelsToMeters(circle.radius);

        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.density = collider.density;
        shapeDef.material.friction = collider.friction;
        shapeDef.material.restitution = collider.restitution;
        shapeDef.material.rollingResistance = collider.rollingResistance;
        shapeDef.material.tangentSpeed = collider.tangentSpeed;
        shapeDef.filter = collider.filter;
        shapeDef.isSensor = collider.isSensor;

        shapeDef.userData = reinterpret_cast<void*>(
            static_cast<uintptr_t>(entity)
        );
        collider.shapeId = b2CreateCircleShape(
            rb.bodyId, &shapeDef, &circleShape
        );
        if (r.all_of<Name>(entity)) {
            Name nameBody = r.get<Name>(entity);
            std::cout << "[CollisionSystem] Created circle collider for entity " 
                      << nameBody.name << "\n";
        }
    }
}

static void createCapsuleColliders(entt::registry &r) {
    auto capsuleView = r.view<Components::RigidBodyComponent, Components::ColliderComponent, Components::CapsuleColliderComponent>();

    for (auto entity : capsuleView) {
        auto& rb = capsuleView.get<Components::RigidBodyComponent>(entity);
        if (B2_IS_NULL(rb.bodyId)) {
            continue; // Body not initialized yet
        }

        auto& collider = capsuleView.get<Components::ColliderComponent>(entity);
        if (B2_IS_NON_NULL(collider.shapeId)) {
            continue; // Collider already created
        }
        auto& capsule = capsuleView.get<Components::CapsuleColliderComponent>(entity);

        b2Capsule capsuleShape;
        // Convert centers + radius to meters
        capsuleShape.center1 = b2Vec2{
            pixelsToMeters(capsule.center1.x),
            pixelsToMeters(capsule.center1.y)
        };
        capsuleShape.center2 = b2Vec2{
            pixelsToMeters(capsule.center2.x),
            pixelsToMeters(capsule.center2.y)
        };
        capsuleShape.radius = pixelsToMeters(capsule.radius);

        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.density = collider.density;
        shapeDef.material.friction = collider.friction;
        shapeDef.material.restitution = collider.restitution;
        shapeDef.material.rollingResistance = collider.rollingResistance;
        shapeDef.filter = collider.filter;
        shapeDef.isSensor = collider.isSensor;

        shapeDef.userData = reinterpret_cast<void*>(
            static_cast<uintptr_t>(entity)
        );
        collider.shapeId = b2CreateCapsuleShape(
            rb.bodyId, &shapeDef, &capsuleShape
        );
        if (r.all_of<Name>(entity)) {
            Name nameBody = r.get<Name>(entity);
            std::cout << "[CollisionSystem] Created capsule collider for entity " 
                      << nameBody.name << "\n";
        }
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
        auto& transform = view.get<WorldTransform>(entity);
        auto& rb = view.get<Components::RigidBodyComponent>(entity);

        if (b2Body_IsValid(rb.bodyId)) {
            auto pos = b2Body_GetPosition(rb.bodyId);
            transform.position.x = metersToPixels(pos.x);
            transform.position.y = metersToPixels(pos.y);
            transform.rotation = b2Rot_GetAngle(
                b2Body_GetRotation(rb.bodyId)
            );

            if (_registry->all_of<Name>(entity)) {
                Name nameBody = _registry->get<Name>(entity);
                std::cout << "[CollisionSystem] Entity : " << nameBody.name
                          << " synced to position (" << transform.position.x
                          << ", " << transform.position.y << ") and rotation ("
                          << transform.rotation << " radians)\n";
            }

            // Optional: sync velocities
            // if (registry.all_of<VelocityComponent>(entity)) {
            //     auto& vel = registry.get<VelocityComponent>(entity);
            //     vel.linear = b2Body_GetLinearVelocity(rb.bodyId);
            //     vel.angular = b2Body_GetAngularVelocity(rb.bodyId);
            // }
        }
    }
}

static void processContactBeginEvents(b2ContactEvents &ContactEvents, entt::registry &registry) {
    std::cout << "[CollisionSystem] Processing " 
        << ContactEvents.beginCount << " begin contact events." << std::endl;

    // Process begin contact
    for (int i = 0; i < ContactEvents.beginCount; ++i) {
        auto& event = ContactEvents.beginEvents[i];
        // Convert userData back to entity (guard against null)
        void* userA = b2Shape_GetUserData(event.shapeIdA);
        void* userB = b2Shape_GetUserData(event.shapeIdB);
        if (!userA || !userB) continue;

        auto entityA = static_cast<entt::entity>(
            static_cast<uintptr_t>(reinterpret_cast<uintptr_t>(userA))
        );
        auto entityB = static_cast<entt::entity>(
            static_cast<uintptr_t>(reinterpret_cast<uintptr_t>(userB))
        );

        if (!registry.valid(entityA) || !registry.valid(entityB)) continue;
        if (!registry.all_of<Name>(entityA) || !registry.all_of<Name>(entityB)) continue;

        Name nameA = registry.get<Name>(entityA);
        Name nameB = registry.get<Name>(entityB);

        // Debug for now
        std::cout << "[CollisionSystem] Begin Contact between " << nameA.name
                << " and " << nameB.name << std::endl;
    }
}

static void processContactHitEvents(b2ContactEvents &ContactEvents, entt::registry &registry) {
    std::cout << "[CollisionSystem] Processing " 
        << ContactEvents.hitCount << " hit contact events." << std::endl;

    // Process hit contact
    for (int i = 0; i < ContactEvents.hitCount; ++i) {
        auto& event = ContactEvents.hitEvents[i];
        void* userA = b2Shape_GetUserData(event.shapeIdA);
        void* userB = b2Shape_GetUserData(event.shapeIdB);
        if (!userA || !userB) continue;

        auto entityA = static_cast<entt::entity>(
            static_cast<uintptr_t>(reinterpret_cast<uintptr_t>(userA))
        );
        auto entityB = static_cast<entt::entity>(
            static_cast<uintptr_t>(reinterpret_cast<uintptr_t>(userB))
        );

        if (!registry.valid(entityA) || !registry.valid(entityB)) continue;
        if (!registry.all_of<Name>(entityA) || !registry.all_of<Name>(entityB)) continue;

        Name nameA = registry.get<Name>(entityA);
        Name nameB = registry.get<Name>(entityB);

        // Debug for now
        std::cout << "[CollisionSystem] Hit Contact between " << nameA.name
                << " and " << nameB.name << std::endl;
    }
}

static void processContactEndEvents(b2ContactEvents &ContactEvents, entt::registry &registry) {
    std::cout << "[CollisionSystem] Processing " 
        << ContactEvents.endCount << " end contact events." << std::endl;

    // Process end contact
    for (int i = 0; i < ContactEvents.endCount; ++i) {
        auto& event = ContactEvents.endEvents[i];
        void* userA = b2Shape_GetUserData(event.shapeIdA);
        void* userB = b2Shape_GetUserData(event.shapeIdB);
        if (!userA || !userB) continue;

        auto entityA = static_cast<entt::entity>(
            static_cast<uintptr_t>(reinterpret_cast<uintptr_t>(userA))
        );
        auto entityB = static_cast<entt::entity>(
            static_cast<uintptr_t>(reinterpret_cast<uintptr_t>(userB))
        );

        if (!registry.valid(entityA) || !registry.valid(entityB)) continue;
        if (!registry.all_of<Name>(entityA) || !registry.all_of<Name>(entityB)) continue;

        Name nameA = registry.get<Name>(entityA);
        Name nameB = registry.get<Name>(entityB);

        // Debug for now
        std::cout << "[CollisionSystem] End Contact between " << nameA.name
                << " and " << nameB.name << std::endl;
    }
}

static void processSensorBeginEvents(b2SensorEvents &sensorEvents, entt::registry &registry) {
    std::cout << "[CollisionSystem] Processing " 
              << sensorEvents.beginCount << " begin sensor events." << std::endl;

    // Process sensor begin touch
    for (int i = 0; i < sensorEvents.beginCount; ++i) {
        auto& event = sensorEvents.beginEvents[i];
        void* userSensor = b2Shape_GetUserData(event.sensorShapeId);
        void* userVisitor = b2Shape_GetUserData(event.visitorShapeId);
        if (!userSensor || !userVisitor) continue;

        auto sensorEntity = static_cast<entt::entity>(
            static_cast<uintptr_t>(reinterpret_cast<uintptr_t>(userSensor))
        );
        auto otherEntity = static_cast<entt::entity>(
            static_cast<uintptr_t>(reinterpret_cast<uintptr_t>(userVisitor))
        );

        if (!registry.valid(sensorEntity) || !registry.valid(otherEntity)) continue;
        if (!registry.all_of<Name>(sensorEntity) || !registry.all_of<Name>(otherEntity)) continue;

        Name sensorEntityName = registry.get<Name>(sensorEntity);
        Name otherEntityName = registry.get<Name>(otherEntity);

        // Debug for now
        std::cout << "[CollisionSystem] Sensor Begin Touch between "
                  << sensorEntityName.name << " and " << otherEntityName.name
                  << std::endl;
    }
}

static void processSensorEndEvents(b2SensorEvents &sensorEvents, entt::registry &registry) {
    std::cout << "[CollisionSystem] Processing " 
              << sensorEvents.endCount << " end sensor events." << std::endl;
    // Process sensor end touch
    for (int i = 0; i < sensorEvents.endCount; ++i) {
        auto& event = sensorEvents.endEvents[i];
        void* userSensor = b2Shape_GetUserData(event.sensorShapeId);
        void* userVisitor = b2Shape_GetUserData(event.visitorShapeId);
        if (!userSensor || !userVisitor) continue;

        auto sensorEntity = static_cast<entt::entity>(
            static_cast<uintptr_t>(reinterpret_cast<uintptr_t>(userSensor))
        );
        auto otherEntity = static_cast<entt::entity>(
            static_cast<uintptr_t>(reinterpret_cast<uintptr_t>(userVisitor))
        );

        if (!registry.valid(sensorEntity) || !registry.valid(otherEntity)) continue;
        if (!registry.all_of<Name>(sensorEntity) || !registry.all_of<Name>(otherEntity)) continue;

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
