/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** SceneState Component [header]
*/

#ifndef SCENESTATE_HPP_
#define SCENESTATE_HPP_

#include <cstdint>
#include <unordered_map>

#include "Hylozoa-Engine/Components/Scene/UUID.hpp"

namespace Hylozoa::Components::HylozoaInternal {

/*
 * @struct SceneState
 * @brief Component to track the loading state of scenes.
 *
 * This component maintains a mapping of scene IDs to their current loading
 * state (loaded or unloaded).
 *
 * @var states
 *      A map of scene IDs to their loading state.
 */
struct SceneState {
    enum class State { LOADED, UNLOADED };

    std::unordered_map<UUID, State> states; // state of each scene by ID
};
} // namespace Hylozoa::Components::HylozoaInternal

#endif /* !SCENESTATE_HPP_ */