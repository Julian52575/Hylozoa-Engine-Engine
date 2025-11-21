#include <SDL3/SDL.h>
#include <entt/entt.hpp>

#include <iostream>

#include "Hylozoa-Engine/Components/Rendering/Renderable.hpp"
#include "Renderer.hpp"

namespace Hylozoa::Systems {

Renderer::Renderer() {}

void Renderer::onStart() { std::cout << "[" << this->_name << "] Start\n"; }

void Renderer::onUpdate(float deltaTime) {
  std::shared_ptr<SDL_Renderer> &renderer =
      Hylozoa::SDL::SDL_Manager::getInstance().getRenderer();

  if (not this->_registry) {
    SDL_SetRenderDrawColor(renderer.get(), 148, 0, 211,
                           255); // Ugly debug purple
    SDL_RenderClear(renderer.get());
    SDL_RenderPresent(renderer.get());
    return;
  }
  // Black clear
  SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 255);
  SDL_RenderClear(renderer.get());
  // Shape rendering
  for (auto &entity :
       this->_registry
           ->view<Hylozoa::Components::Rendering::Renderable,
                  Hylozoa::Components::Rendering::RenderableShape>()) {
    auto &worldTransform =
        this->_registry->get<Hylozoa::WorldTransform>(entity);
    auto &renderable =
        this->_registry->get<Hylozoa::Components::Rendering::Renderable>(
            entity);
    auto &shape =
        this->_registry->get<Hylozoa::Components::Rendering::RenderableShape>(
            entity);

    renderShape(worldTransform, renderable, shape);
  }
  // Texture rendering
  for (auto &entity :
       this->_registry
           ->view<Hylozoa::Components::Rendering::Renderable,
                  Hylozoa::Components::Rendering::RenderableTexture>()) {
    auto &worldTransform =
        this->_registry->get<Hylozoa::WorldTransform>(entity);
    auto &renderable =
        this->_registry->get<Hylozoa::Components::Rendering::Renderable>(
            entity);
    auto &texture =
        this->_registry->get<Hylozoa::Components::Rendering::RenderableTexture>(
            entity);

    // std::cout << "worldTransform: " << worldTransform.position.x << ", " <<
    // worldTransform.position.y << std::endl;
    renderTexture(worldTransform, renderable, texture);
  }
  SDL_RenderPresent(renderer.get());
}

void Renderer::onEnd() { std::cout << "[" << this->_name << "] End\n"; }

inline void Renderer::renderShape(
    const Hylozoa::WorldTransform &transform,
    const Hylozoa::Components::Rendering::Renderable &renderable,
    const Hylozoa::Components::Rendering::RenderableShape &shape) {
  if (!renderable.visible) {
    return;
  }
  switch (shape.type) {
  case Hylozoa::Components::Rendering::RenderableShape::ShapeType::Circle:
    renderShapeCircle(transform, renderable, shape);
    break;
  case Hylozoa::Components::Rendering::RenderableShape::ShapeType::Rectangle:
    renderShapeRectangle(transform, renderable, shape);
    break;
  default:
    break;
  }
}

inline void Renderer::renderShapeCircle(
    const Hylozoa::WorldTransform &transform,
    const Hylozoa::Components::Rendering::Renderable &sprite,
    const Hylozoa::Components::Rendering::RenderableShape &shape) {
  std::shared_ptr<SDL_Renderer> &renderer =
      Hylozoa::SDL::SDL_Manager::getInstance().getRenderer();
  const Hylozoa::Components::Rendering::RenderableShape::CircleSpecs
      &circleSpecs = std::get<
          Hylozoa::Components::Rendering::RenderableShape::CircleSpecs>(
          shape.specs);
  int radius = static_cast<int>(circleSpecs.radius);

  SDL_SetRenderDrawColor(renderer.get(), sprite.color.r, sprite.color.g,
                         sprite.color.b, sprite.color.a);
  for (int w = 0; w < radius * 2; w++) {
    for (int h = 0; h < radius * 2; h++) {
      int dx = radius - w; // horizontal offset
      int dy = radius - h; // vertical offset

      if ((dx * dx + dy * dy) <= (radius * radius)) {
        SDL_RenderPoint(renderer.get(), transform.position.x + dx,
                        transform.position.y + dy);
      }
    }
  }
}

inline void Renderer::renderShapeRectangle(
    const Hylozoa::WorldTransform &transform,
    const Hylozoa::Components::Rendering::Renderable &sprite,
    const Hylozoa::Components::Rendering::RenderableShape &shape) {
  std::shared_ptr<SDL_Renderer> &renderer =
      Hylozoa::SDL::SDL_Manager::getInstance().getRenderer();
  const Hylozoa::Components::Rendering::RenderableShape::RectangleSpecs
      &rectSpecs = std::get<
          Hylozoa::Components::Rendering::RenderableShape::RectangleSpecs>(
          shape.specs);

  fillRect.x = transform.position.x;
  fillRect.y = transform.position.y;
  fillRect.w = rectSpecs.width;
  fillRect.h = rectSpecs.height;
  SDL_SetRenderDrawColor(renderer.get(), sprite.color.r, sprite.color.g,
                         sprite.color.b, sprite.color.a);
  SDL_RenderFillRect(renderer.get(), &fillRect);
}

inline void Renderer::renderTexture(
    const Hylozoa::WorldTransform &transform,
    const Hylozoa::Components::Rendering::Renderable &renderable,
    Hylozoa::Components::Rendering::RenderableTexture &texture) {
  if (!renderable.visible) {
    return;
  }
  SDL_FRect destRect;
  texture.getSDLRect(destRect);
  destRect.x = transform.position.x;
  destRect.y = transform.position.y;
  destRect.w *= transform.scale.x * renderable.scale;
  destRect.h *= transform.scale.y * renderable.scale;

  SDL_Texture *sdlTexture = texture.getSDLTexture();
  std::shared_ptr<SDL_Renderer> &renderer =
      Hylozoa::SDL::SDL_Manager::getInstance().getRenderer();

  if (not SDL_RenderTexture(renderer.get(), sdlTexture, NULL, &destRect)) {
    SDL_Log("Couldn't render texture: %s", SDL_GetError());
  }
}

} // namespace Hylozoa::Systems