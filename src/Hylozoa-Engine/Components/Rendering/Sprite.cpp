#include "Sprite.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_surface.h>

#include <cstdint>
#include <optional>
#include <string>
#include <variant>

#include "Hylozoa-Engine/Components/Color.hpp"
#include "Hylozoa-Engine/Components/Vector2.hpp"

namespace Hylozoa::Components::Rendering {

bool Sprite::initCircle() {
#warning Untested AI code below
  SpriteSpecs::ShapeSpecs::CircleSpecs circleSpecs =
      std::get<SpriteSpecs::ShapeSpecs::CircleSpecs>(
          this->specs.shapeSpecs.specifics);

  int diameter = static_cast<int>(circleSpecs.radius * 2 * this->specs.scale.x);
  this->sdlRect.w = diameter;
  this->sdlRect.h = diameter;
  return true;
}

bool Sprite::initRectangle() {
#warning Untested AI code below
  SpriteSpecs::ShapeSpecs::RectangleSpecs rectangleSpecs =
      std::get<SpriteSpecs::ShapeSpecs::RectangleSpecs>(
          this->specs.shapeSpecs.specifics);

  this->sdlRect.w =
      static_cast<int>(rectangleSpecs.width * this->specs.scale.x);
  this->sdlRect.h =
      static_cast<int>(rectangleSpecs.height * this->specs.scale.y);
  return true;
}

bool Sprite::initTexture() {
  SpriteSpecs::ShapeSpecs::TextureSpecs textureSpecs =
      std::get<SpriteSpecs::ShapeSpecs::TextureSpecs>(
          this->specs.shapeSpecs.specifics);
  std::string texture_path = SDL_GetBasePath() + textureSpecs.texturePath;
  SDL_Surface *surface = SDL_LoadPNG(texture_path.c_str());

  if (!surface) {
    SDL_Log("Couldn't load bitmap: %s", SDL_GetError());
    return false;
  }
  this->sdlRect.w = surface->w;
  this->sdlRect.h = surface->h;
  this->sdlTexture =
      SDL_CreateTextureFromSurface(this->renderer.get(), surface);
  if (!this->sdlTexture) {
    SDL_Log("Couldn't create static texture: %s", SDL_GetError());
    return false;
  }
  SDL_DestroySurface(
      surface); /* done with this, the texture has a copy of the pixels now. */
  return true;
}

Sprite::Sprite(std::shared_ptr<SDL_Renderer> &renderer,
               const SpriteSpecs &specs)
    : renderer(renderer), specs(specs) {
  switch (this->specs.shapeSpecs.type) {
  case SpriteType::Circle:
    initCircle();
    break;
  case SpriteType::Rectangle:
    initRectangle();
    break;
  case SpriteType::Texture:
    initTexture();
    break;
  default:
    throw std::runtime_error("Unknown SpriteType");
  }
}

} // namespace Hylozoa::Components::Rendering