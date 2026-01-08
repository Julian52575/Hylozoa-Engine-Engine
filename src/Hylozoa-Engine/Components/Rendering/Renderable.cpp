#include "Renderable.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_surface.h>

#include <cstdint>
#include <iostream>
#include <optional>
#include <string>
#include <variant>

#include "Hylozoa-Engine/Components/Color.hpp"
#include "Hylozoa-Engine/Components/Vector2.hpp"
#include "Hylozoa-Engine/SDL/SDL_Manager.hpp"

namespace Hylozoa::Components::Rendering {

void RenderableTexture::init(const RenderableTexture::Specs &textureSpecs) {
    std::string texture_path = SDL_GetBasePath() + textureSpecs.texturePath;
    SDL_Surface *surface = SDL_LoadPNG(texture_path.c_str());
    std::shared_ptr<SDL_Renderer> &renderer =
        Hylozoa::SDL::SDL_Manager::getInstance().getRenderer();

    if (!surface) {
        SDL_Log("Couldn't load bitmap: %s", SDL_GetError());
#warning TODO handle error with exceptions
        return;
    }
    this->sdlRect.w = surface->w;
    this->sdlRect.h = surface->h;
    this->sdlTexture = SDL_CreateTextureFromSurface(renderer.get(), surface);
    if (!this->sdlTexture) {
        SDL_Log("Couldn't create static texture: %s", SDL_GetError());
        return;
    }
    /* done with this, the texture has a copy of the pixels now. */
    SDL_DestroySurface(surface);
}

RenderableTexture::RenderableTexture(const std::string &texturePath) {
    Specs specs;

    specs.texturePath = texturePath;
    init(specs);
}

RenderableTexture::RenderableTexture(
    const RenderableTexture::Specs &textureSpecs) {
    init(textureSpecs);
}

RenderableTexture::~RenderableTexture() {
    if (this->sdlTexture) {
        SDL_DestroyTexture(this->sdlTexture);
    }
}

RenderableTexture::RenderableTexture(RenderableTexture &&other) noexcept
    : sdlTexture(other.sdlTexture), texturePath(std::move(other.texturePath)),
      origin(other.origin), scale(other.scale),
      animation(std::move(other.animation)), sdlRect(other.sdlRect) {
    // Prevent other from destroying the texture
    other.sdlTexture = nullptr;
}

RenderableTexture &
RenderableTexture::operator=(RenderableTexture &&other) noexcept {
    if (this != &other) {
        // Destroy our current texture
        if (this->sdlTexture) {
            SDL_DestroyTexture(this->sdlTexture);
        }

        // Take ownership of other's texture
        sdlTexture = other.sdlTexture;
        texturePath = std::move(other.texturePath);
        origin = other.origin;
        scale = other.scale;
        animation = std::move(other.animation);
        sdlRect = other.sdlRect;

        // Prevent other from destroying the texture
        other.sdlTexture = nullptr;
    }
    return *this;
}

} // namespace Hylozoa::Components::Rendering