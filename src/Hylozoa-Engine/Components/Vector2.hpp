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

using Vector2i = SDL_Point;
using Vector2f = SDL_FPoint;

} // namespace Hylozoa::Components

inline std::ostream &operator<<(std::ostream &os,
                                const Hylozoa::Components::Vector2i &v) {
  return os << "Vector2i(" << v.x << ", " << v.y << ")";
}

inline std::ostream &operator<<(std::ostream &os,
                                const Hylozoa::Components::Vector2f &v) {
  return os << "Vector2f(" << v.x << ", " << v.y << ")";
}