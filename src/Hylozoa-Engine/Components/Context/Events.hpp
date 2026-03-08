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


namespace Hylozoa::Components::HylozoaInternal {

/*
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

struct OnSceneLoaded {
    UUID sceneId{0};
};

struct OnSceneUnloaded {
    UUID sceneId{0};
};

struct OnNoiseEvent {
    entt::entity source{entt::null};
    std::string noiseName;
    glm::vec2 position{0.0f, 0.0f};
    float intensity{0.0f};
};

struct EventsDispatcher {
    entt::dispatcher dispatcher;
};

} // namespace Hylozoa::Components::HylozoaInternal

#endif /* !EVENTS_HPP_ */