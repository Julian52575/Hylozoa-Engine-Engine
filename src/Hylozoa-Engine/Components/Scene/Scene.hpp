/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Scene Related Components [header]
*/

#ifndef SCENE_HPP_
#define SCENE_HPP_

#include "UUID.hpp"

#include <cstdint>
#include <string>

namespace Hylozoa {
namespace Components {
namespace HylozoaInternal {
/**
 * @struct Scene
 * @brief Link an entity to a scene.
 *
 * This component associates an entity with a specific scene
 *
 * @var name
 *      The name of the scene.
 * @var id
 *     The unique identifier of the scene.
 */
struct SceneTag {
    std::string name; // Name of the scene
    UUID id;          // Unique scene identifier
};

/**
 * @struct SceneActiveTag
 * @brief Tag component to indicate if an entity is part of the active scene.
 */
struct SceneActiveTag {};

struct Deserializing {};

} // namespace HylozoaInternal
} // namespace Components
} // namespace Hylozoa
#endif /* !SCENE_HPP_ */