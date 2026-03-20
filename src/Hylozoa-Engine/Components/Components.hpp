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

/**
 * @namespace Hylozoa::Components
 * @brief Contains various components related to physics simulation in the
 * Hylozoa Engine.
 */
namespace Hylozoa::Components {}

/**
 * @namespace Hylozoa::Components::HylozoaInternal
 * @brief Contains internal components used by the Hylozoa Engine for managing
 * engine state, events, input, and other core functionalities. NOT FOR EXTERNAL
 * USER USE!
 */
namespace Hylozoa::Components::HylozoaInternal {}

// ======== CONTEXT COMPONENTS ========
#include "Context/EngineContext.hpp"
#include "Context/Events.hpp"
#include "Context/Input.hpp"
#include "Context/SceneState.hpp"
#include "Context/Time.hpp"

// ======= AUDIO COMPONENTS ========
#include "Audio/Listener.hpp"

// ======== CAMERA COMPONENTS ========
#include "Camera/Camera.hpp"
#include "Camera/Serializer.hpp"

// ======== PHYSICS COMPONENTS ========
#include "Physics/Physics.hpp"
#include "Physics/Serializer.hpp"

// ======== RENDERING COMPONENTS ========
#include "Rendering/Renderable.hpp"
#include "Rendering/Serializer.hpp"

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
