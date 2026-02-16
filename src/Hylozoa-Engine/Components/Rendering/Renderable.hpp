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

#include "Hylozoa-Engine/Core/LayerManager.hpp"
#include "Hylozoa-Engine/Core/Resources.hpp"

#include <SDL3/SDL.h>
// #include <SDL3/SDL_rect.h>
// #include <SDL3/SDL_texture.h>

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <variant>

namespace Hylozoa::Components {

namespace Rendering {

enum class SpriteType { Texture, Rectangle, Circle };

/**
 * Core sprite component - handles visibility, layering, and basic rendering
 * properties Used by: All visible entities
 */
struct Renderable {
    Renderable() = default;
    ~Renderable() = default;

    Hylozoa::Components::Color color;
    bool visible{true};
    LayerBit layer{0};        // default layer 0 (Default)
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
 * Animation component - handles sprite sheet animation data
 * WILL BE REFACTORED SOON
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

struct Sprite {
    std::string textureName;
    SDL_FPoint scale{1.0f, 1.0f};
    SDL_Point origin{0, 0};
};

} // namespace Rendering

namespace HylozoaInternal {

struct RenderTexture {
    std::weak_ptr<Resources::Texture> texture;
    SDL_FRect destRect{0, 0, 0, 0};
    SDL_Texture* getTexture() const { return texture.lock() ? texture.lock()->get() : nullptr; }
    void getRect(SDL_FRect& rect) const { rect = destRect; }
};

}

} // namespace Hylozoa::Components

