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
 * Core sprite component - handles visibility, layering, and basic rendering
 * properties Used by: All visible entities
 */
struct Sprite {
  Sprite() = default;
  ~Sprite() = default;

  Hylozoa::Components::Color color;
  float scale{1.0f};
  bool visible{true};
  int layer{0};
  float transparency{1.0f}; // 0.0 = fully transparent, 1.0 = fully opaque
};

/**
 * Shape component - for rendering primitive geometric shapes
 * Used by: Entities rendered as shapes (rectangles, circles, etc.)
 */
struct Shape {
  enum class ShapeType { Rectangle, Circle };

  struct RectangleSpecs {
    float width{30.0f};
    float height{30.0f};
  };

  struct CircleSpecs {
    float radius{30.0f};
  };

  ShapeType type{ShapeType::Rectangle};
  std::variant<RectangleSpecs, CircleSpecs> specs{RectangleSpecs{}};

  SDL_Color outlineColor{0, 0, 0, 255};
  float outlineThickness{1.0f};
};

/**
 * Texture component - for rendering textured sprites
 * Used by: Entities with textures (will be refactored to use resource manager)
 */
class Texture {
public:
  struct Specs {
    std::string texturePath;
    SDL_Point originOffset{0, 0};
    SDL_FPoint textureScale{1.0f, 1.0f};
  };

  Texture(std::shared_ptr<SDL_Renderer> &renderer,
          const Texture::Specs &textureSpecs);
  ~Texture();
  SDL_Texture *getSDLTexture() { return this->sdlTexture; }
  const SDL_Texture *getSDLTexture() const { return this->sdlTexture; }
  SDL_FRect getSDLRect() const { return this->sdlRect; }
  void getSDLRect(SDL_FRect &dest) const { dest = this->sdlRect; }

private:
  std::shared_ptr<SDL_Renderer> &renderer;
  SDL_Texture *sdlTexture{nullptr};
  std::string texturePath; // TODO: move to resource manager, keep only texture
                           // name reference
  SDL_Point origin{0, 0};
  float scale{1.0f};

  // Runtime state
  SDL_FRect sdlRect{0, 0, 0, 0};
};

/**
 * Animation component - handles sprite sheet animation data
 * Used by: Animated entities (works with Texture component)
 */
struct Animation {
  SDL_FRect frameRect{0, 0, 0, 0};
  float frameRectWidth{0};
  float frameRectHeight{0};
  int frameCount{1};
  float frameDuration{0.1f};          // seconds per frame
  SDL_FPoint frameDisplacement{0, 0}; // offset between frames

  // Runtime state
  int currentFrame{0};
  float elapsedTime{0.0f};
};

} // namespace Hylozoa::Components::Rendering