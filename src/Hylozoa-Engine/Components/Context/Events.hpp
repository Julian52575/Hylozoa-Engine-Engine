/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Events Context Component [header]
*/

#ifndef EVENTS_HPP_
#define EVENTS_HPP_

#include "Hylozoa-Engine/Components/Scene/UUID.hpp"
#include "entt/entt.hpp"
#include <glm/glm.hpp>

namespace Hylozoa {
class Entity;
}

namespace Hylozoa::Components::HylozoaInternal {

/**
 * @struct Events
 * @brief Component to store engine event flags.
 *
 * This component holds flags for various engine events
 * such as quit, pause, and resume requests.
 */
struct EngineEvents {
    bool shouldQuit{false};      // Quit event flag
    bool pauseRequested{false};  // Pause event flag
    bool resumeRequested{false}; // Resume event flag
};

/**
 * @struct OnSceneLoaded
 * @brief OnSceneLoaded event data structure.
 *
 * This structure is used to pass data when a scene is loaded, containing the
 * UUID of the loaded scene.
 */
struct OnSceneLoaded {
    UUID sceneId{0};
};

/**
 * @struct OnSceneUnloaded
 * @brief OnSceneUnloaded event data structure.
 *
 * This structure is used to pass data when a scene is unloaded, containing the
 * UUID of the unloaded scene.
 */
struct OnSceneUnloaded {
    UUID sceneId{0};
};

/**
 * @struct OnNoiseEvent
 * @brief OnNoiseEvent event data structure.
 *
 * This structure is used to pass data when a noise event occurs, containing the
 * source entity, noise name, position, and intensity of the noise.
 */
struct OnNoiseEvent {
    entt::entity source{entt::null};
    std::string noiseName{""};
    glm::vec2 position{0.0f, 0.0f};
};

struct NoiseInfo {
    std::string noiseName{""};
    glm::vec2 position{0.0f, 0.0f};
    glm::vec2 direction{0.0f, 0.0f};
};

/**
 * @struct EventsDispatcher
 * @brief EventsDispatcher component to hold the EnTT dispatcher for engine
 * events.
 *
 * This component contains an instance of `entt::dispatcher` which is used to
 * manage and dispatch events within the engine. Systems can subscribe to this
 * dispatcher to listen for events such as scene loading, unloading, and noise
 * events (for now, more to come).
 */
struct EventsDispatcher {
    entt::dispatcher dispatcher;
};

} // namespace Hylozoa::Components::HylozoaInternal

#endif /* !EVENTS_HPP_ */