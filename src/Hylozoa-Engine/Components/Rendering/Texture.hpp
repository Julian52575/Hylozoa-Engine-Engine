/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Sprite component for rending entities
*/
#pragma once

#include "Hylozoa-Engine/Components/Color.hpp"
#include "Hylozoa-Engine/Components/Vector2.hpp"
#include <cstdint>
#include <optional>
#include <string>

namespace Hylozoa::Components::Rendering {

/**
 * @addtogroup Rendering
 * @namespace Hylozoa::Components::Rendering
 * @struct Texture
 * @brief The Texture.
 */
struct Texture {
  /**
   * @brief The path to the spritesheet to apply to the sprite. Leave empty for
   * no texture.
   */
  std::string texturePath;
  /**
   * @brief The sprite's color.
   */
  Hylozoa::Components::Color color;
  /**
   * @brief The transparency value of the sprite.
   * 0 = fully visible, 255 = fully transparent.
   * Default: 0
   */
  uint8_t transparency{0};
  /**
   * @brief The offset of the sprite's origin (starting from the top left).
   * Default: 0x, 0y.
   */
  Hylozoa::Components::Vector2 originOffset{0, 0};
  /**
   * @brief The scaling to apply to the texture.
   * Default: 1x, 1y.
   */
  Hylozoa::Components::Vector2 textureScale{1, 1};
  /**
   * @struct SpriteAnimationSpecs
   * @brief The sprite animation specifications.
   */
  struct SpriteAnimationSpecs {
    /**
     * @brief The position of the first sprite's frame.
     * Default: 0x, 0y.
     */
    Hylozoa::Components::Vector2 frameRectXY{0, 0};
    /**
     * @brief The size of each frame.
     * Default: 100x, 100y.
     */
    Hylozoa::Components::Vector2 frameRectWidthHeight{100, 100};
    /**
     * @brief The number of frame in the spritesheet.
     * Default: 0
     */
    uint16_t frameCount = 1;
    /**
     * @brief The duration in seconds of each sprite.
     * Default: 0.8 (seconds).
     */
    float frameDuration = 0.8;
    /**
     * @brief The displacement between each frame in the spritesheet.
     * Default: 0x, 0y
     */
    Hylozoa::Components::Vector2 frameDisplacement = {0, 0};
  };
  /**
   * @brief The sprite animation specifications. If not set, the sprite is
   * static.
   */
  std::optional<SpriteAnimationSpecs> animation;
};

} // namespace Hylozoa::Components::Rendering