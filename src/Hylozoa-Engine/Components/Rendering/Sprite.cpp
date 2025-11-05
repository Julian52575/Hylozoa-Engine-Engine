#include "Sprite.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_surface.h>

#include <cstdint>
#include <optional>
#include <string>
#include <variant>

#include "Hylozoa-Engine/Components/Color.hpp"
#include "Hylozoa-Engine/Components/Vector2.hpp"

namespace Hylozoa::Components::Rendering
{

  Texture::Texture(std::shared_ptr<SDL_Renderer> &renderer, const Texture::Specs& textureSpecs)
      : renderer(renderer)
  {
    std::string texture_path = SDL_GetBasePath() + textureSpecs.texturePath;
    SDL_Surface *surface = SDL_LoadPNG(texture_path.c_str());

    if (!surface) {
      SDL_Log("Couldn't load bitmap: %s", SDL_GetError());
      #warning TODO handle error with exceptions
      return;
    }
    this->sdlRect.w = surface->w;
    this->sdlRect.h = surface->h;
    this->sdlTexture =
        SDL_CreateTextureFromSurface(this->renderer.get(), surface);
    if (!this->sdlTexture) {
      SDL_Log("Couldn't create static texture: %s", SDL_GetError());
      return;
    }
    /* done with this, the texture has a copy of the pixels now. */
    SDL_DestroySurface(surface);
  }

  Texture::~Texture()
  {
    if (this->sdlTexture) {
      SDL_DestroyTexture(this->sdlTexture);
    }
  }

} // namespace Hylozoa::Components::Rendering