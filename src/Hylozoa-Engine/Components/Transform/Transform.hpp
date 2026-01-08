/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Transform Component [header]
*/

#ifndef TRANSFORM_HPP_
#define TRANSFORM_HPP_

#include <SDL3/SDL.h>
#include <cmath>
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <numbers>
#include <ostream>
#include <unordered_set>

namespace Hylozoa {
/*
 * @brief Converts radians to degrees.
 */
inline float radToDeg(float rad) { return rad * 180.0f / std::numbers::pi; }

/*
 * @brief Converts degrees to radians.
 */
inline float degToRad(float deg) { return deg * (std::numbers::pi / 180.0f); }

namespace Components {
/*
 * @struct Name
 * @brief Component to store the name of an entity.
 * This component holds a string representing the name assigned to an entity.
 *
 * @var string name
 */
struct Name {
    std::string name;
};

/*
 * @struct LocalTransform
 * @brief Component to store local transformation data.
 * This component holds the position, scale, and rotation of an entity relative
 * to its parent.
 *
 * @var glm::vec2 position
 *      The local position of the entity.
 * @var glm::vec2 scale
 *      The local scale of the entity.
 * @var float rotation
 *      The local rotation of the entity in radians.
 */
struct LocalTransform {
    glm::vec2 position;
    glm::vec2 scale;
    float rotation; // in RADIANS !!
};

/*
 * @struct WorldTransform
 * @brief Component to store world transformation data.
 * This component holds the position, scale, and rotation of an entity in world
 * space.
 *
 * @var glm::vec2 position
 *      The world position of the entity.
 * @var glm::vec2 scale
 *      The world scale of the entity.
 * @var float rotation
 *      The world rotation of the entity in radians.
 */
struct WorldTransform {
    glm::vec2 position;
    glm::vec2 scale;
    float rotation; // in RADIANS !!
};

/*
 * @struct Parent
 * @brief Component to store the parent entity reference.
 * This component holds a reference to the parent entity of the current entity.
 *
 * @var entt::entity entity
 *      The parent entity reference.
 */
struct Parent {
    entt::entity entity{entt::null};
};

/*
 * @namespace HylozoaInternal
 * @brief Internal components for managing entity relationships and
 * transformations.
 */
namespace HylozoaInternal {
/*
 * @struct LocalToWorld
 * @brief Component to store the local-to-world transformation matrix.
 */
struct LocalToWorld {
    glm::mat3 matrix;
};

/*
 * @struct Children
 * @brief Component to store the set of child entities.
 */
struct Children {
    std::unordered_set<entt::entity> childrens;
};
} // namespace HylozoaInternal
} // namespace Components

inline glm::mat3 translation(const glm::vec2 &t) {
    return glm::mat3(1, 0, t.x, 0, 1, t.y, 0, 0, 1);
}

inline glm::mat3 rotation(float angle) {
    float c = std::cos(angle);
    float s = std::sin(angle);

    return glm::mat3(c, -s, 0, s, c, 0, 0, 0, 1);
}

inline glm::mat3 scale(const glm::vec2 &s) {
    return glm::mat3(s.x, 0, 0, 0, s.y, 0, 0, 0, 1);
}

inline glm::mat3 fromTransform(const Components::LocalTransform &tr) {
    return translation(tr.position) * rotation(tr.rotation) * scale(tr.scale);
}

inline Components::WorldTransform toWorldTransform(const glm::mat3 &m) {
    Components::WorldTransform wt;

    wt.position = glm::vec2(m[2][0], m[2][1]);

    // rotation from the matrix (atan2 of direction X axis)
    wt.rotation = std::atan2(m[0][1], m[0][0]);

    wt.scale.x = glm::length(glm::vec2(m[0][0], m[0][1]));
    wt.scale.y = glm::length(glm::vec2(m[1][0], m[1][1]));

    return wt;
}

} // namespace Hylozoa

#endif /* !TRANSFORM_HPP_ */