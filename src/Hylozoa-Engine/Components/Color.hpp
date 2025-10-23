/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Color structure
*/
#pragma once

#include <cstdint>

namespace Hylozoa::Components {

/**
* @addtogroup Rendering
* @namespace Hylozoa::Components
* @class Color
* @brief A structure representing RGBA color.
*/
struct Color {
  uint8_t red{255};
  uint8_t green{255};
  uint8_t blue{255};
  /*
  * @brief The alpha (transparency) value of the color.
  * 0 = fully visible, 255 = fully transparent.
  */
  uint8_t alpha{0};
};

} // namespace Hylozoa::Components