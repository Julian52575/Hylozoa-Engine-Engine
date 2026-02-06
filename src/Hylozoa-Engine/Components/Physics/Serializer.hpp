/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Physic related components serialization [header]
*/

#pragma once

#include "Physics.hpp"

#include <nlohmann/json.hpp>
// for convenience
using json = nlohmann::json;

namespace Hylozoa::Components {

inline void to_json(json &j, const RigidBodyComponent &rbc) {
    std::string bodyType = "static";

    if (rbc.type == b2_dynamicBody)
        bodyType = "dynamic";
    else if (rbc.type == b2_kinematicBody)
        bodyType = "kinematic";

    j = json{{"type", bodyType},
             {"gravityScale", rbc.gravityScale},
             {"linearDamping", rbc.linearDamping},
             {"angularDamping", rbc.angularDamping},
             {"fixedRotation", rbc.fixedRotation},
             {"isBullet", rbc.isBullet},
             {"isAwake", rbc.isAwake},
             {"isEnabled", rbc.isEnabled}};
}

inline void from_json(const json &j, RigidBodyComponent &rbc) {
    std::string bodyType = j.value("type", "static");

    if (bodyType == "dynamic")
        rbc.type = b2_dynamicBody;
    else if (bodyType == "kinematic")
        rbc.type = b2_kinematicBody;
    else
        rbc.type = b2_staticBody;

    rbc.gravityScale = j.value("gravityScale", 1.0f);
    rbc.linearDamping = j.value("linearDamping", 0.0f);
    rbc.angularDamping = j.value("angularDamping", 0.0f);
    rbc.fixedRotation = j.value("fixedRotation", false);
    rbc.isBullet = j.value("isBullet", false);
    rbc.isAwake = j.value("isAwake", true);
    rbc.isEnabled = j.value("isEnabled", true);

    rbc.linearVelocity.x = 0.0f;
    rbc.linearVelocity.y = 0.0f;
}

inline void to_json(json &j, const ColliderComponent &cc) {
    j = json{{"density", cc.density},
             {"friction", cc.friction},
             {"restitution", cc.restitution},
             {"rollingResistance", cc.rollingResistance},
             {"tangentSpeed", cc.tangentSpeed},
             {"isSensor", cc.isSensor},
             {"enableContactEvents", cc.enableContactEvents},
             {"enableSensorEvents", cc.enableSensorEvents},
             {"enableHitEvents", cc.enableHitEvents}};
}

inline void from_json(const json &j, ColliderComponent &cc) {
    cc.density = j.value("density", 1.0f);
    cc.friction = j.value("friction", 0.6f);
    cc.restitution = j.value("restitution", 0.0f);
    cc.rollingResistance = j.value("rollingResistance", 0.0f);
    cc.tangentSpeed = j.value("tangentSpeed", 0.0f);
    cc.isSensor = j.value("isSensor", false);
    cc.enableContactEvents = j.value("enableContactEvents", false);
    cc.enableSensorEvents = j.value("enableSensorEvents", false);
    cc.enableHitEvents = j.value("enableHitEvents", false);
}

inline void to_json(json &j, const BoxColliderComponent &bcc) {
    j = json{{"width", bcc.halfWidth * 2.0f},
             {"height", bcc.halfHeight * 2.0f}};
}

inline void from_json(const json &j, BoxColliderComponent &bcc) {
    float width = j.value("width", 2.0f);
    float height = j.value("height", 2.0f);

    bcc.halfWidth = width / 2.0f;
    bcc.halfHeight = height / 2.0f;
}

inline void to_json(json &j, const CircleColliderComponent &ccc) {
    j = json{{"radius", ccc.radius},
             {"offset", {{"x", ccc.offset.x}, {"y", ccc.offset.y}}}};
}

inline void from_json(const json &j, CircleColliderComponent &ccc) {
    ccc.radius = j.value("radius", 0.5f);

    const auto &offset = j.value("offset", json::object());
    ccc.offset.x = offset.value("x", 0.0f);
    ccc.offset.y = offset.value("y", 0.0f);
}

inline void to_json(json &j, const CapsuleColliderComponent &ccc) {
    j = json{{"center1", {{"x", ccc.center1.x}, {"y", ccc.center1.y}}},
             {"center2", {{"x", ccc.center2.x}, {"y", ccc.center2.y}}},
             {"radius", ccc.radius}};
}

inline void from_json(const json &j, CapsuleColliderComponent &ccc) {
    const auto &center1 = j.value("center1", json::object());
    ccc.center1.x = center1.value("x", 0.0f);
    ccc.center1.y = center1.value("y", 0.0f);

    const auto &center2 = j.value("center2", json::object());
    ccc.center2.x = center2.value("x", 0.0f);
    ccc.center2.y = center2.value("y", 0.0f);

    ccc.radius = j.value("radius", 0.5f);
}

} // namespace Hylozoa::Components