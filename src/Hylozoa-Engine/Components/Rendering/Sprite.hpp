/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Sprite component for rending entities
** The sprite can be a rectangle, a circle, or a free texture
*/
#pragma once

#include "Hylozoa-Engine/Components/Color.hpp"
#include "Hylozoa-Engine/Components/Vector2.hpp"
#include <cstdint>
#include <string>
#include <variant>

namespace Hylozoa::Components::Rendering {

enum class SpriteType { Texture, Rectangle, Circle };

/**
 * @addtogroup Rendering
 * @namespace Hylozoa::Components::Rendering
 * @struct Sprite
 * @brief The Sprite component.
 */
struct Sprite {
  /**
   * @class SpriteSpecs
   * @brief A sprite specifications structure to define how to create a sprite.
   * The 'animation' field can be configured to match the spritesheet.
   */
  struct SpriteSpecs {
    /**
     * @brief Circle-specific data.
     * @struct CircleData
     */
    struct CircleData {
      /**
       * @brief The radius of the circle.
       * Default: 1.0f
       */
      float radius{1.0f};
    };
    /**
     * @brief Rectangle-specific data.
     * @struct RectangleData
     */
    struct RectangleData {
      /**
       * @brief The width of the rectangle.
       * Default: 192
       */
      float width{192};
      /**
       * @brief The height of the rectangle.
       * Default: 108
       */
      float height{108};
    };
    /**
     * @brief Texture data.
     * @struct SpriteData
     */
    struct SpriteData {
      // Empty for now, but can be extended in the future if needed
    };
    /**
     * @brief This structure manages data for the Circle and Rectangle sprite.
     * @struct ShapeData
     */
    struct ShapeData {
      /**
       * @brief The shape-specific data (circle radius, rectangle size, or
       * sprite).
       */
      std::variant<SpriteData, CircleData, RectangleData> specifics =
          SpriteData{};
      /**
       * @brief The size of the shape's outline.
       * Default: 1
       */
      float outlineThickness = 1;
      /**
       * @brief The shape's outline color.
       * Default: White {255, 255, 255}
       */
      Hylozoa::Components::Color outlineColor;
    };

  }; // struct SpriteSpecs

  /**
   * @brief The sprite specs.
   */
  SpriteSpecs specs;

  /**
   * @brief The sprite's color.
   */
  Hylozoa::Components::Color color;

  /**
   * @brief The sprite scale.
   */
  Hylozoa::Components::Vector2 scale{1.0f, 1.0f};

  /**
   * @brief Is the sprite visible?
   */
  bool visible{true};

  /**
   * @brief The rendering layer of the sprite.
   * Lower layers are rendered first.
   */
  int layer{0}; // Rendering order

  /**
   * @brief The sprite's transparency.
   * Default: 0 (fully visible), 255 = fully transparent.
   */
  uint8_t transparency{0};
};

} // namespace Hylozoa::Components::Rendering