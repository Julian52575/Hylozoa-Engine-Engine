/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Color structure
*/
#pragma once

#include <cstdint>

namespace Hylozoa::Components {

struct Color {
  uint8_t red{255};
  uint8_t green{255};
  uint8_t blue{255};
  uint8_t alpha{0};
};

} // namespace Hylozoa::Components