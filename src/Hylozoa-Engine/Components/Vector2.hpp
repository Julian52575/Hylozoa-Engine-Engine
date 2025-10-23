/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Sprite component for rending entities
*/
#pragma once

#include <glm/vec2.hpp> // glm::vec3
#include <ostream>

namespace Hylozoa::Components {

using Vector2 = glm::vec2;

} // namespace Hylozoa::Components

inline std::ostream &operator<<(std::ostream &os,
                                const Hylozoa::Components::Vector2 &v) {
  return os << "Vector2(" << v.x << ", " << v.y << ")";
}