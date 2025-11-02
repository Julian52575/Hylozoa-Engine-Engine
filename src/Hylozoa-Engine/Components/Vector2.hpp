/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Sprite component for rending entities
*/
#pragma once

// import sdl vector2
#include <SDL3/SDL_rect.h>
#include <ostream>

namespace Hylozoa::Components {

using Vector2 = SDL_FPoint;

} // namespace Hylozoa::Components

inline std::ostream &operator<<(std::ostream &os,
                                const Hylozoa::Components::Vector2 &v) {
  return os << "Vector2(" << v.x << ", " << v.y << ")";
}