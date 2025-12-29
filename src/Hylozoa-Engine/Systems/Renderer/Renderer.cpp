// Renderer.cpp
#include "Renderer.hpp"
#include "Hylozoa-Engine/Components/Physics/Physics.hpp"
#include "Hylozoa-Engine/Components/Scene/Scene.hpp"
#include <algorithm>
#include <glm/vec2.hpp>
#include <iostream>
#include <vector>

namespace Hylozoa::Systems {

Renderer::Renderer() {}

void Renderer::onStart() { std::cout << "[" << this->_name << "] Start\n"; }

void Renderer::onUpdate(float deltaTime) {
  std::shared_ptr<SDL_Renderer> &renderer =
      Hylozoa::SDL::SDL_Manager::getInstance().getRenderer();

  if (!this->_registry) {
    SDL_SetRenderDrawColor(renderer.get(), 148, 0, 211,
                           255); // Ugly debug purple
    SDL_RenderClear(renderer.get());
    SDL_RenderPresent(renderer.get());
    return;
  }

  SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 255);
  SDL_RenderClear(renderer.get());

  // collect camera entities
  auto camView = this->_registry->view<Hylozoa::Components::Camera,
                                       Hylozoa::Components::WorldTransform,
                                       Hylozoa::Components::HylozoaInternal::SceneActiveTag>();
  std::vector<entt::entity> cameras;
  cameras.reserve(std::distance(camView.begin(), camView.end()));
  for (auto e : camView)
    cameras.push_back(e);

  // sort cameras by order (low -> high)

  std::sort(cameras.begin(), cameras.end(),
            [&](entt::entity a, entt::entity b) {
              const Hylozoa::Components::Camera &ca =
                  camView.get<Hylozoa::Components::Camera>(a);
              const Hylozoa::Components::Camera &cb =
                  camView.get<Hylozoa::Components::Camera>(b);
              return ca.order < cb.order;
            });

  if (cameras.empty()) {
    // no cameras: nothing to render
    std::cout << "[" << this->_name
              << "] Warning: No camera found in the scene. Nothing to "
                 "render.\n"; // debug message
    SDL_RenderPresent(renderer.get());
    return;
  }

  // Render for each camera
  for (auto camEntity : cameras) {
    const auto &cam = camView.get<Hylozoa::Components::Camera>(camEntity);
    const auto &camTransform =
        camView.get<Hylozoa::Components::WorldTransform>(camEntity);

    renderSingleCamera(cam, camTransform);
  }

  SDL_RenderPresent(renderer.get());
}

void Renderer::onEnd() { std::cout << "[" << this->_name << "] End\n"; }

/**
 * @brief Renders all renderable entities visible to a single camera.
 *
 * This function iterates over all renderable entities in the registry and
 * renders only those that are visible to the specified camera based on its
 * culling mask.
 *
 * @param camera The camera component used for rendering.
 * @param cameraTransform The world transform of the camera.
 *
 **/
void Renderer::renderSingleCamera(
    const Hylozoa::Components::Camera &camera,
    const Hylozoa::Components::WorldTransform &cameraTransform) {

  // === SHAPES ===
  auto shapeView =
      this->_registry->view<Hylozoa::Components::Rendering::Renderable,
                            Hylozoa::Components::Rendering::RenderableShape,
                            Hylozoa::Components::WorldTransform,
                            Hylozoa::Components::HylozoaInternal::SceneActiveTag
                            >();

  for (auto entity : shapeView) {
    const auto &renderable =
        shapeView.get<Hylozoa::Components::Rendering::Renderable>(entity);
    const auto &shape =
        shapeView.get<Hylozoa::Components::Rendering::RenderableShape>(entity);
    const auto &transform =
        shapeView.get<Hylozoa::Components::WorldTransform>(entity);

    if ((renderable.layer & camera.cullingMask) == 0)
      continue;

    renderShape(transform, renderable, shape, camera, cameraTransform);
  }

  // === TEXTURES ===
  auto texView =
      this->_registry->view<Hylozoa::Components::Rendering::Renderable,
                            Hylozoa::Components::Rendering::RenderableTexture,
                            Hylozoa::Components::WorldTransform,
                            Hylozoa::Components::HylozoaInternal::SceneActiveTag
                            >();

  for (auto entity : texView) {
    const auto &renderable =
        texView.get<Hylozoa::Components::Rendering::Renderable>(entity);
    auto &texture =
        texView.get<Hylozoa::Components::Rendering::RenderableTexture>(entity);
    const auto &transform =
        texView.get<Hylozoa::Components::WorldTransform>(entity);

    if (!renderable.visible)
      continue;
    if ((renderable.layer & camera.cullingMask) == 0)
      continue;

    renderTexture(transform, renderable, texture, camera, cameraTransform);
  }
}

void Renderer::renderShape(
    const Hylozoa::Components::WorldTransform &transform,
    const Hylozoa::Components::Rendering::Renderable &renderable,
    const Hylozoa::Components::Rendering::RenderableShape &shape,
    const Hylozoa::Components::Camera &camera,
    const Hylozoa::Components::WorldTransform &cameraTransform) {

  if (!renderable.visible)
    return;

  switch (shape.type) {
  case Hylozoa::Components::Rendering::RenderableShape::ShapeType::Circle:
    renderShapeCircle(transform, renderable, shape, camera, cameraTransform);
    break;

  case Hylozoa::Components::Rendering::RenderableShape::ShapeType::Rectangle:
    renderShapeRectangle(transform, renderable, shape, camera, cameraTransform);
    break;
  }
}

void Renderer::renderShapeCircle(
    const Hylozoa::Components::WorldTransform &transform,
    const Hylozoa::Components::Rendering::Renderable &sprite,
    const Hylozoa::Components::Rendering::RenderableShape &shape,
    const Hylozoa::Components::Camera &camera,
    const Hylozoa::Components::WorldTransform &cameraTransform) {

  std::shared_ptr<SDL_Renderer> &renderer =
      Hylozoa::SDL::SDL_Manager::getInstance().getRenderer();

  const auto &circleSpecs =
      std::get<Hylozoa::Components::Rendering::RenderableShape::CircleSpecs>(
          shape.specs);

  float finalRadius =
      circleSpecs.radius * transform.scale.x * sprite.scale * camera.zoom;

  glm::vec2 center = worldToView(transform.position, camera, cameraTransform);

  SDL_SetRenderDrawColor(renderer.get(), sprite.color.r, sprite.color.g,
                         sprite.color.b, sprite.color.a);

  int r = static_cast<int>(finalRadius);

  for (int w = -r; w <= r; w++) {
    for (int h = -r; h <= r; h++) {
      if (w * w + h * h <= r * r) {
        SDL_RenderPoint(renderer.get(), static_cast<int>(center.x + w),
                        static_cast<int>(center.y + h));
      }
    }
  }
}

void Renderer::renderShapeRectangle(
    const Hylozoa::Components::WorldTransform &transform,
    const Hylozoa::Components::Rendering::Renderable &sprite,
    const Hylozoa::Components::Rendering::RenderableShape &shape,
    const Hylozoa::Components::Camera &camera,
    const Hylozoa::Components::WorldTransform &cameraTransform) {

  std::shared_ptr<SDL_Renderer> &renderer =
      Hylozoa::SDL::SDL_Manager::getInstance().getRenderer();

  const auto &rectSpecs =
      std::get<Hylozoa::Components::Rendering::RenderableShape::RectangleSpecs>(
          shape.specs);

  SDL_FRect fillRect;
  glm::vec2 screenPos =
      worldToView(transform.position, camera, cameraTransform);

  fillRect.w = rectSpecs.width * transform.scale.x * sprite.scale * camera.zoom;
  fillRect.h =
      rectSpecs.height * transform.scale.y * sprite.scale * camera.zoom;
  fillRect.y = screenPos.y - (fillRect.h * 0.5f);
  fillRect.x = screenPos.x - (fillRect.w * 0.5f);

  SDL_SetRenderDrawColor(renderer.get(), sprite.color.r, sprite.color.g,
                         sprite.color.b, sprite.color.a);
  SDL_RenderFillRect(renderer.get(), &fillRect);
}

void Renderer::renderTexture(
    const Hylozoa::Components::WorldTransform &transform,
    const Hylozoa::Components::Rendering::Renderable &renderable,
    Hylozoa::Components::Rendering::RenderableTexture &texture,
    const Hylozoa::Components::Camera &camera,
    const Hylozoa::Components::WorldTransform &cameraTransform) {

  SDL_FRect destRect;
  texture.getSDLRect(destRect);

  glm::vec2 screenPos =
      worldToView(transform.position, camera, cameraTransform);

  destRect.w *= transform.scale.x * renderable.scale;
  destRect.h *= transform.scale.y * renderable.scale;

  destRect.x = screenPos.x - (destRect.w * 0.5f);
  destRect.y = screenPos.y - (destRect.h * 0.5f);

  SDL_Texture *sdlTexture = texture.getSDLTexture();
  std::shared_ptr<SDL_Renderer> &renderer =
      Hylozoa::SDL::SDL_Manager::getInstance().getRenderer();

  if (!SDL_RenderTexture(renderer.get(), sdlTexture, nullptr, &destRect)) {
    SDL_Log("Couldn't render texture: %s", SDL_GetError());
  }
}

/*
 * @brief Converts world coordinates to view (screen) coordinates based on the
 *camera.
 *
 * This function transforms a position in world space to screen space, taking
 *into account the camera's position, zoom level, and viewport size. If the
 *camera is designated as a UI camera, the world position is treated as already
 *being in screen coordinates.
 *
 * @param worldPos The position in world coordinates to be converted.
 * @param camera The camera component used for the conversion.
 * @param cameraTransform The world transform of the camera.
 * @return The corresponding position in view (screen) coordinates.
 **/
glm::vec2 Renderer::worldToView(
    const glm::vec2 &worldPos, const Hylozoa::Components::Camera &camera,
    const Hylozoa::Components::WorldTransform &cameraTransform) {
  // UI camera: treat worldPos as already screen coords
  if (camera.isUI) {
    return worldPos;
  }

  // Normal camera: center camera on its transform.position, apply zoom, then
  // center to viewport
  glm::vec2 centered = (worldPos - cameraTransform.position) * camera.zoom;
  glm::vec2 screen = centered + camera.viewportSize * 0.5f;
  return screen;
}

} // namespace Hylozoa::Systems
