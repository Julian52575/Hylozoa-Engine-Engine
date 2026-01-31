/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Global Components header
*/

#ifndef COMPONENTS_HPP_
#define COMPONENTS_HPP_

#include <nlohmann/json.hpp>
using namespace nlohmann;

// ======== CONTEXT COMPONENTS ========
#include "Context/EngineContext.hpp"
#include "Context/Events.hpp"
#include "Context/Input.hpp"
#include "Context/SceneState.hpp"
#include "Context/Time.hpp"

// ======== CAMERA COMPONENTS ========
#include "Camera/Serializer.hpp"
#include "Camera/Camera.hpp"

// ======== PHYSICS COMPONENTS ========
#include "Physics/Serializer.hpp"
#include "Physics/Physics.hpp"

// ======== RENDERING COMPONENTS ========
#include "Rendering/Renderable.hpp"

// ======== SCENE COMPONENTS ========
#include "Scene/Scene.hpp"
#include "Scene/UUID.hpp"

// ======== TRANSFORM COMPONENTS ========
#include "Transform/Serializer.hpp"
#include "Transform/Transform.hpp"

template <typename Component>
void serializeIfPresent(entt::registry &registry, entt::entity entity,
                        json &entityJson, const char *key) {
    if (!registry.all_of<Component>(entity))
        return;

    entityJson[key] = registry.get<Component>(entity);
}

template <typename Component>
void deserializeIfPresent(entt::registry &registry, entt::entity entity,
                          const json &entityJson, const char *key) {
    if (!entityJson.contains(key))
        return;

    Component comp = entityJson.at(key).get<Component>();
    registry.emplace_or_replace<Component>(entity, comp);
}

#endif
