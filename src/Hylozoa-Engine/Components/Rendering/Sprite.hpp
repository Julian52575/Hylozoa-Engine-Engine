/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Sprite component for rending entities
*/
#pragma once

#include "Hylozoa-Engine/Components/Color.hpp"
#include <cstdint>
#include <string>
#include <variant>

namespace Hylozoa::Components::Rendering {

enum class SpriteType { Texture, Rectangle, Circle };

struct Sprite {

  struct TextureData {
    uint32_t textureId;
    std::string path;
  };
  struct RectangleData {
    float width;
    float height;
  };
  struct CircleData {
    float radius;
    uint32_t segments;
  };

  SpriteType type{SpriteType::Rectangle};
  std::variant<TextureData, RectangleData, CircleData>
      data; // std::variant is a C++17 feature similar to unions

  // Visual properties
  Hylozoa::Components::Color color; // RGBA
  float width{1.0f};
  float height{1.0f};

  // Rendering properties
  bool visible{true};
  int layer{0}; // Rendering order
  float opacity{1.0f};
};

} // namespace Hylozoa::Components::Rendering