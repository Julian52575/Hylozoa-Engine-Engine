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
struct Renderable {
    Renderable() = default;
    ~Renderable() = default;

    Hylozoa::Components::Color color;
    float scale{1.0f};
    bool visible{true};
    uint32_t layer = 1u << 0; // default layer 0 (LAYER_WORLD)
    float transparency{1.0f}; // 0.0 = fully transparent, 1.0 = fully opaque
};

/**
 * Shape component - for rendering primitive geometric shapes
 * Used by: Entities rendered as shapes (rectangles, circles, etc.)
 */
struct RenderableShape {
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
class RenderableTexture {
  public:
    /**
     * Animation component - handles sprite sheet animation data
     * Used by: Animated entities (works with Texture component)
     */
    struct AnimationSpecs {
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

    struct Specs {
        std::string texturePath;
        SDL_Point originOffset{0, 0};
        SDL_FPoint textureScale{1.0f, 1.0f};
        bool cropsToRenderable{true};
    };

    RenderableTexture(const std::string &texturePath);
    RenderableTexture(const RenderableTexture::Specs &textureSpecs);
    ~RenderableTexture();

    // Delete copy (textures can't be copied)
    RenderableTexture(const RenderableTexture &) = delete;
    RenderableTexture &operator=(const RenderableTexture &) = delete;

    // Allow move
    RenderableTexture(RenderableTexture &&other) noexcept;
    RenderableTexture &operator=(RenderableTexture &&other) noexcept;
    SDL_Texture *getSDLTexture() { return this->sdlTexture; }
    const SDL_Texture *getSDLTexture() const { return this->sdlTexture; }
    SDL_FRect getSDLRect() const { return this->sdlRect; }
    void getSDLRect(SDL_FRect &dest) const { dest = this->sdlRect; }

  private:
    void init(const RenderableTexture::Specs &textureSpecs);
    SDL_Texture *sdlTexture{nullptr};
    std::string texturePath; // TODO: move to resource manager, keep only
                             // texture name reference
    SDL_Point origin{0, 0};
    float scale{1.0f};
    std::optional<AnimationSpecs> animation;

    // Runtime state
    SDL_FRect sdlRect{0, 0, 0, 0};
};

} // namespace Hylozoa::Components::Rendering