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

#include <SDL3/SDL.h>
// #include <SDL3/SDL_rect.h>
// #include <SDL3/SDL_texture.h>

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <variant>

namespace Hylozoa::Components::Rendering {

enum class SpriteType { Texture, Rectangle, Circle };

/**
 * @class SpriteSpecs
 * @brief A sprite specifications structure to define how to create a sprite.
 * The 'animation' field can be configured to match the spritesheet.
 */
struct SpriteSpecs {

  /**
   * @brief This structure manages data for the Circle and Rectangle sprite.
   * @struct ShapeSpecs
   */
  struct ShapeSpecs {
    /**
     * @brief Circle-specific data.
     * @struct CircleData
     */
    struct CircleSpecs {
      /**
       * @brief The radius of the circle.
       * Default: 1.0f
       */
      float radius{1.0f};
    }; // struct CircleData

    /**
     * @brief Rectangle-specific data.
     * @struct RectangleData
     */

    struct RectangleSpecs {
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
    }; // struct RectangleData

    /**
     * @struct TextureSpecs
     * @brief The specs to create a Sprite's Texture.
     */
    struct TextureSpecs {
      /**
       * @brief The path to the spritesheet to apply to the sprite. Leave empty
       * for no texture.
       */
      std::string texturePath;
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
    }; // struct TextureSpecs
    /**
     * @brief The type of the shape (texture, circle or rectangle).
     * Default: Circle
     */
    SpriteType type{SpriteType::Circle};
    /**
     * @brief The shape-specific data (circle radius, rectangle size, or
     * sprite).
     */
    std::variant<TextureSpecs, CircleSpecs, RectangleSpecs> specifics =
        CircleSpecs{};
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
  }; // struct ShapeSpecs

  /**
   * @brief The shape specs of the sprite.
   */
  ShapeSpecs shapeSpecs;
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
}; // struct SpriteSpecs

/**
 * @addtogroup Rendering
 * @namespace Hylozoa::Components::Rendering
 * @struct Sprite
 * @brief The Sprite component.
 */
class Sprite {
public:
  Sprite(std::shared_ptr<SDL_Renderer> &renderer, const SpriteSpecs &specs);
  ~Sprite() = default;
  SDL_Texture *getSDLTexture() const { return sdlTexture; }
  const SDL_Rect *getSDLRect() const { return &sdlRect; }
  const SpriteSpecs &getSpecs() const { return specs; }

private:
  SpriteSpecs specs;
  std::shared_ptr<SDL_Renderer> &renderer;
  SDL_Texture *sdlTexture{nullptr};
  SDL_Rect sdlRect{0, 0, 0, 0};

private:
  bool initCircle();
  bool initRectangle();
  bool initTexture();
};

} // namespace Hylozoa::Components::Rendering