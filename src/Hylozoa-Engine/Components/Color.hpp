/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Color structure
*/
#pragma once

#include <SDL3/SDL.h>

namespace Hylozoa::Components {

/**
 * @addtogroup Rendering
 * @namespace Hylozoa::Components
 * @class Color
 * @brief A remapping of SDL_Color
 */
using Color = SDL_Color;

inline constexpr Color Black() { return Color{0, 0, 0, 255}; }

inline constexpr Color White() { return Color{255, 255, 255, 255}; }

} // namespace Hylozoa::Components