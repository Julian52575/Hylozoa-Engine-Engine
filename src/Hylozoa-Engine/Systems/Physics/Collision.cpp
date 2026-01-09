/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Collisions System [Source]
*/

#include "Hylozoa-Engine/Components/Physics/Physics.hpp"
#include "Hylozoa-Engine/Components/Transform/Transform.hpp"

#include "Collision.hpp"

#include <cstdint>
#include <entt/entt.hpp>
#include <iostream>

namespace Hylozoa {

void CollisionSystem::createBodies() {
    auto view = _registry->view<Components::LocalTransform,
                                Components::RigidBodyComponent>();

    for (auto entity : view) {
        auto &transform = view.get<Components::LocalTransform>(entity);
        auto &rb = view.get<Components::RigidBodyComponent>(entity);

        if (B2_IS_NON_NULL(rb.bodyId))
            continue;

        b2BodyDef bodyDef = b2DefaultBodyDef();
        bodyDef.type = rb.type;

        // Convert pixel position (center-based) -> meters for Box2D
        bodyDef.position = b2Vec2{transform.position.x / PIXELS_PER_METER,
                                  transform.position.y / PIXELS_PER_METER};
        bodyDef.rotation = b2MakeRot(transform.rotation);

        bodyDef.linearDamping = rb.linearDamping;
        bodyDef.angularDamping = rb.angularDamping;
        bodyDef.gravityScale = rb.gravityScale;
        bodyDef.fixedRotation = rb.fixedRotation;
        bodyDef.isBullet = rb.isBullet;
        bodyDef.isAwake = rb.isAwake;
        bodyDef.isEnabled = rb.isEnabled;

        bodyDef.userData =
            reinterpret_cast<void *>(static_cast<uintptr_t>(entity));

        rb.bodyId = b2CreateBody(m_world, &bodyDef);
        if (_registry->all_of<Components::Name>(entity)) {
            Components::Name nameBody =
                _registry->get<Components::Name>(entity);
            std::cout << "[CollisionSystem] Created body for entity "
                      << nameBody.name << "\n";
        }
    }
}

static void createBoxColliders(entt::registry &r) {
    auto boxView =
        r.view<Components::RigidBodyComponent, Components::ColliderComponent,
               Components::BoxColliderComponent>();

    for (auto entity : boxView) {

        auto &rb = boxView.get<Components::RigidBodyComponent>(entity);
        if (B2_IS_NULL(rb.bodyId))
            continue;

        auto &collider = boxView.get<Components::ColliderComponent>(entity);
        if (B2_IS_NON_NULL(collider.shapeId))
            continue;

        auto &box = boxView.get<Components::BoxColliderComponent>(entity);

        // Convert half extents from pixels -> meters
        b2Polygon poly = b2MakeBox(box.halfWidth / PIXELS_PER_METER,
                                   box.halfHeight / PIXELS_PER_METER);

        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.density = collider.density;
        shapeDef.material.friction = collider.friction;
        shapeDef.material.restitution = collider.restitution;
        shapeDef.material.rollingResistance = collider.rollingResistance;
        shapeDef.material.tangentSpeed = collider.tangentSpeed;
        // shapeDef.filter = collider.filter;
        shapeDef.isSensor = collider.isSensor;
        shapeDef.enableContactEvents = collider.enableContactEvents;
        shapeDef.enableSensorEvents = collider.enableSensorEvents;
        shapeDef.enableHitEvents = collider.enableHitEvents;

        shapeDef.userData =
            reinterpret_cast<void *>(static_cast<uintptr_t>(entity));

        collider.shapeId = b2CreatePolygonShape(rb.bodyId, &shapeDef, &poly);
        if (r.all_of<Components::Name>(entity)) {
            Components::Name nameBody = r.get<Components::Name>(entity);
            std::cout << "[CollisionSystem] Created box collider for entity "
                      << nameBody.name << "\n";
        }
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
        auto &circle =
            circleView.get<Components::CircleColliderComponent>(entity);

        b2Circle circleShape;
        // Convert offset (pixels) -> meters and radius (pixels) -> meters
        circleShape.center = b2Vec2{circle.offset.x / PIXELS_PER_METER,
                                    circle.offset.y / PIXELS_PER_METER};
        circleShape.radius = circle.radius / PIXELS_PER_METER;

        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.density = collider.density;
        shapeDef.material.friction = collider.friction;
        shapeDef.material.restitution = collider.restitution;
        shapeDef.material.rollingResistance = collider.rollingResistance;
        shapeDef.material.tangentSpeed = collider.tangentSpeed;
        // shapeDef.filter = collider.filter; // don't know how to properly use
        // this for now please avoid
        shapeDef.isSensor = collider.isSensor;
        shapeDef.enableContactEvents = collider.enableContactEvents;
        shapeDef.enableSensorEvents = collider.enableSensorEvents;
        shapeDef.enableHitEvents = collider.enableHitEvents;

        shapeDef.userData =
            reinterpret_cast<void *>(static_cast<uintptr_t>(entity));
        collider.shapeId =
            b2CreateCircleShape(rb.bodyId, &shapeDef, &circleShape);
        if (r.all_of<Components::Name>(entity)) {
            Components::Name nameBody = r.get<Components::Name>(entity);
            std::cout << "[CollisionSystem] Created circle collider for entity "
                      << nameBody.name << "\n";
        }
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
        // Convert centers + radius to meters
        capsuleShape.center1 = b2Vec2{capsule.center1.x / PIXELS_PER_METER,
                                      capsule.center1.y / PIXELS_PER_METER};
        capsuleShape.center2 = b2Vec2{capsule.center2.x / PIXELS_PER_METER,
                                      capsule.center2.y / PIXELS_PER_METER};
        capsuleShape.radius = capsule.radius / PIXELS_PER_METER;

        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.density = collider.density;
        shapeDef.material.friction = collider.friction;
        shapeDef.material.restitution = collider.restitution;
        shapeDef.material.rollingResistance = collider.rollingResistance;
        // shapeDef.filter = collider.filter; // don't know how to properly use
        // this for now please avoid
        shapeDef.isSensor = collider.isSensor;
        shapeDef.enableContactEvents = collider.enableContactEvents;
        shapeDef.enableSensorEvents = collider.enableSensorEvents;
        shapeDef.enableHitEvents = collider.enableHitEvents;

        shapeDef.userData =
            reinterpret_cast<void *>(static_cast<uintptr_t>(entity));
        collider.shapeId =
            b2CreateCapsuleShape(rb.bodyId, &shapeDef, &capsuleShape);
        if (r.all_of<Components::Name>(entity)) {
            Components::Name nameBody = r.get<Components::Name>(entity);
            std::cout
                << "[CollisionSystem] Created capsule collider for entity "
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

void CollisionSystem::syncECStoBox2D() {
    auto view = _registry->view<Components::RigidBodyComponent>();
    for (auto entity : view) {
        auto &rb = view.get<Components::RigidBodyComponent>(entity);

        if (!b2Body_IsValid(rb.bodyId))
            continue;

        b2Body_SetLinearVelocity(rb.bodyId, rb.linearVelocity);
    }
}

// Sync Box2D transforms back to ECS
void CollisionSystem::syncBox2DtoECS() {
    auto view = _registry->view<Components::RigidBodyComponent>();

    for (auto entity : view) {
        auto &rb = view.get<Components::RigidBodyComponent>(entity);

        if (!b2Body_IsValid(rb.bodyId))
            continue;

        b2Vec2 pos = b2Body_GetPosition(rb.bodyId); // meters
        float angle = b2Rot_GetAngle(b2Body_GetRotation(rb.bodyId));
        b2Vec2 vel = b2Body_GetLinearVelocity(rb.bodyId); // meters/sec

        // Retrieve the local scale if present (keeps existing behavior)
        glm::vec2 scale(1.f, 1.f);
        if (_registry->all_of<Components::LocalTransform>(entity)) {
            scale = _registry->get<Components::LocalTransform>(entity).scale;
        }

        // Convert Box2D meters -> pixels for world transform
        Components::WorldTransform wt{
            glm::vec2(pos.x * PIXELS_PER_METER, pos.y * PIXELS_PER_METER),
            scale, angle};

        _registry->emplace_or_replace<Components::WorldTransform>(entity, wt);

        // Keep rb.linearVelocity in Box2D units since only used internally.
        // (for now at least)
        rb.linearVelocity = b2Vec2{vel.x, vel.y};

        if (_registry->all_of<Components::Name>(entity)) {
            auto name = _registry->get<Components::Name>(entity).name;
            if (name == "Player" || name == "debug") {
                printf("%4.2f %4.2f %4.2f vel %4.2f %4.2f\n",
                       pos.x * PIXELS_PER_METER, pos.y * PIXELS_PER_METER,
                       angle, vel.x, vel.y);
            }
        }
    }
}

static void processContactBeginEvents(b2ContactEvents &ContactEvents,
                                      entt::registry &registry) {
    std::cout << "[CollisionSystem] Processing " << ContactEvents.beginCount
              << " begin contact events." << std::endl;

    // Process begin contact
    for (int i = 0; i < ContactEvents.beginCount; ++i) {
        auto &event = ContactEvents.beginEvents[i];

        // Convert userData back to entity (guard against null)
        void *userA = b2Shape_GetUserData(event.shapeIdA);
        void *userB = b2Shape_GetUserData(event.shapeIdB);
        if (!userA || !userB)
            continue;

        auto entityA = static_cast<entt::entity>(
            static_cast<uintptr_t>(reinterpret_cast<uintptr_t>(userA)));
        auto entityB = static_cast<entt::entity>(
            static_cast<uintptr_t>(reinterpret_cast<uintptr_t>(userB)));

        if (!registry.valid(entityA) || !registry.valid(entityB))
            continue;
        if (!registry.all_of<Components::Name>(entityA) ||
            !registry.all_of<Components::Name>(entityB))
            continue;

        Components::Name nameA = registry.get<Components::Name>(entityA);
        Components::Name nameB = registry.get<Components::Name>(entityB);

        // Debug for now
        std::cout << "[CollisionSystem] Begin Contact between " << nameA.name
                  << " and " << nameB.name << std::endl;
    }
}

static void processContactHitEvents(b2ContactEvents &ContactEvents,
                                    entt::registry &registry) {
    // std::cout << "[CollisionSystem] Processing "
    //     << ContactEvents.hitCount << " hit contact events." << std::endl;

    // Process hit contact
    for (int i = 0; i < ContactEvents.hitCount; ++i) {
        auto &event = ContactEvents.hitEvents[i];
        void *userA = b2Shape_GetUserData(event.shapeIdA);
        void *userB = b2Shape_GetUserData(event.shapeIdB);
        if (!userA || !userB)
            continue;

        auto entityA = static_cast<entt::entity>(
            static_cast<uintptr_t>(reinterpret_cast<uintptr_t>(userA)));
        auto entityB = static_cast<entt::entity>(
            static_cast<uintptr_t>(reinterpret_cast<uintptr_t>(userB)));

        if (!registry.valid(entityA) || !registry.valid(entityB))
            continue;
        if (!registry.all_of<Components::Name>(entityA) ||
            !registry.all_of<Components::Name>(entityB))
            continue;

        Components::Name nameA = registry.get<Components::Name>(entityA);
        Components::Name nameB = registry.get<Components::Name>(entityB);

        // Debug for now
        // std::cout << "[CollisionSystem] Hit Contact between " << nameA.name
        //         << " and " << nameB.name << std::endl;
    }
}

static void processContactEndEvents(b2ContactEvents &ContactEvents,
                                    entt::registry &registry) {
    std::cout << "[CollisionSystem] Processing " << ContactEvents.endCount
              << " end contact events." << std::endl;

    // Process end contact
    for (int i = 0; i < ContactEvents.endCount; ++i) {
        auto &event = ContactEvents.endEvents[i];
        void *userA = b2Shape_GetUserData(event.shapeIdA);
        void *userB = b2Shape_GetUserData(event.shapeIdB);
        if (!userA || !userB)
            continue;

        auto entityA = static_cast<entt::entity>(
            static_cast<uintptr_t>(reinterpret_cast<uintptr_t>(userA)));
        auto entityB = static_cast<entt::entity>(
            static_cast<uintptr_t>(reinterpret_cast<uintptr_t>(userB)));

        if (!registry.valid(entityA) || !registry.valid(entityB))
            continue;
        if (!registry.all_of<Components::Name>(entityA) ||
            !registry.all_of<Components::Name>(entityB))
            continue;

        Components::Name nameA = registry.get<Components::Name>(entityA);
        Components::Name nameB = registry.get<Components::Name>(entityB);

        // Debug for now
        std::cout << "[CollisionSystem] End Contact between " << nameA.name
                  << " and " << nameB.name << std::endl;
    }
}

static void processSensorBeginEvents(b2SensorEvents &sensorEvents,
                                     entt::registry &registry) {
    // Process sensor begin touch
    for (int i = 0; i < sensorEvents.beginCount; ++i) {
        auto &event = sensorEvents.beginEvents[i];
        void *userSensor = b2Shape_GetUserData(event.sensorShapeId);
        void *userVisitor = b2Shape_GetUserData(event.visitorShapeId);
        if (!userSensor || !userVisitor)
            continue;

        auto sensorEntity = static_cast<entt::entity>(
            static_cast<uintptr_t>(reinterpret_cast<uintptr_t>(userSensor)));
        auto otherEntity = static_cast<entt::entity>(
            static_cast<uintptr_t>(reinterpret_cast<uintptr_t>(userVisitor)));

        if (!registry.valid(sensorEntity) || !registry.valid(otherEntity))
            continue;
        if (!registry.all_of<Components::Name>(sensorEntity) ||
            !registry.all_of<Components::Name>(otherEntity))
            continue;

        Components::Name sensorEntityName =
            registry.get<Components::Name>(sensorEntity);
        Components::Name otherEntityName =
            registry.get<Components::Name>(otherEntity);

        // Debug for now
        std::cout << "[CollisionSystem] Sensor Begin Touch between "
                  << sensorEntityName.name << " and " << otherEntityName.name
                  << std::endl;
    }
}

static void processSensorEndEvents(b2SensorEvents &sensorEvents,
                                   entt::registry &registry) {
    // Process sensor end touch
    for (int i = 0; i < sensorEvents.endCount; ++i) {
        auto &event = sensorEvents.endEvents[i];
        void *userSensor = b2Shape_GetUserData(event.sensorShapeId);
        void *userVisitor = b2Shape_GetUserData(event.visitorShapeId);
        if (!userSensor || !userVisitor)
            continue;

        auto sensorEntity = static_cast<entt::entity>(
            static_cast<uintptr_t>(reinterpret_cast<uintptr_t>(userSensor)));
        auto otherEntity = static_cast<entt::entity>(
            static_cast<uintptr_t>(reinterpret_cast<uintptr_t>(userVisitor)));

        if (!registry.valid(sensorEntity) || !registry.valid(otherEntity))
            continue;
        if (!registry.all_of<Components::Name>(sensorEntity) ||
            !registry.all_of<Components::Name>(otherEntity))
            continue;

        Components::Name sensorEntityName =
            registry.get<Components::Name>(sensorEntity);
        Components::Name otherEntityName =
            registry.get<Components::Name>(otherEntity);
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