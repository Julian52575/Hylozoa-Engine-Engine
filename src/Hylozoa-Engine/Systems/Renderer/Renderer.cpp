// Renderer.cpp
#include "Renderer.hpp"
#include "Hylozoa-Engine/Components/Physics/Physics.hpp"
#include "Hylozoa-Engine/Components/Scene/Scene.hpp"
#include "Hylozoa-Engine/Core/Entities/Entity.hpp"
#include "Hylozoa-Engine/Core/Settings.hpp"
#include <algorithm>
#include <glm/vec2.hpp>
#include <iostream>
#include <vector>

namespace Hylozoa::Systems {

void Renderer::onStart() {
    if (Hylozoa::Settings::getInstance().getSettings().verbose) {
        std::cout << "[" << this->_name << "] Start\n";
    }
}

void Renderer::onUpdate(float deltaTime) {
    std::shared_ptr<SDL_Renderer> &renderer =
        Hylozoa::SDL::SDL_Manager::getInstance().getRenderer();

    SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 255);
    SDL_RenderClear(renderer.get());

    // collect camera entities
    auto camView = this->_registry.view<
        Hylozoa::Components::Camera, Hylozoa::Components::WorldTransform,
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
        if (Hylozoa::Settings::getInstance().getSettings().verbose) {
            /*std::cout << "[" << this->_name
                      << "] Warning: No camera found in the scene. Nothing to "
                         "render.\n"; // debug message*/
        }
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
        this->_registry
            .view<Hylozoa::Components::Rendering::Renderable,
                  Hylozoa::Components::Rendering::RenderableShape,
                  Hylozoa::Components::WorldTransform,
                  Hylozoa::Components::HylozoaInternal::SceneActiveTag>();

    for (auto entity : shapeView) {
        const auto &renderable =
            shapeView.get<Hylozoa::Components::Rendering::Renderable>(entity);
        const auto &shape =
            shapeView.get<Hylozoa::Components::Rendering::RenderableShape>(
                entity);
        const auto &transform =
            shapeView.get<Hylozoa::Components::WorldTransform>(entity);

        if (!camera.cullingMask.contains(renderable.layer))
            continue;

        renderShape(transform, renderable, shape, camera, cameraTransform);
    }

    // === TEXTURES ===
    auto texView =
        this->_registry
            .view<Hylozoa::Components::Rendering::Renderable,
                  Hylozoa::Components::Rendering::Sprite,
                  Hylozoa::Components::HylozoaInternal::RenderTexture,
                  Hylozoa::Components::WorldTransform,
                  Hylozoa::Components::HylozoaInternal::SceneActiveTag>();

    for (auto entity : texView) {
        const auto &renderable =
            texView.get<Hylozoa::Components::Rendering::Renderable>(entity);
        const auto &sprite =
            texView.get<Hylozoa::Components::Rendering::Sprite>(entity);
        auto &texture =
            texView.get<Hylozoa::Components::HylozoaInternal::RenderTexture>(
                entity);
        const auto &transform =
            texView.get<Hylozoa::Components::WorldTransform>(entity);
        if (!renderable.visible)
            continue;
        if (!camera.cullingMask.contains(renderable.layer))
            continue;

        updateTexture(transform, renderable, sprite, texture, camera,
                      cameraTransform);
        renderTexture(transform, renderable, sprite, texture, camera,
                      cameraTransform);
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
        renderShapeCircle(transform, renderable, shape, camera,
                          cameraTransform);
        break;

    case Hylozoa::Components::Rendering::RenderableShape::ShapeType::Rectangle:
        renderShapeRectangle(transform, renderable, shape, camera,
                             cameraTransform);
        break;
    }
}

void Renderer::renderShapeCircle(
    const Hylozoa::Components::WorldTransform &transform,
    const Hylozoa::Components::Rendering::Renderable &renderable,
    const Hylozoa::Components::Rendering::RenderableShape &shape,
    const Hylozoa::Components::Camera &camera,
    const Hylozoa::Components::WorldTransform &cameraTransform) {

    std::shared_ptr<SDL_Renderer> &renderer =
        Hylozoa::SDL::SDL_Manager::getInstance().getRenderer();

    const auto &circleSpecs =
        std::get<Hylozoa::Components::Rendering::RenderableShape::CircleSpecs>(
            shape.specs);

    float finalRadius = circleSpecs.radius * transform.scale.x * camera.zoom;

    glm::vec2 center = worldToView(transform.position, camera, cameraTransform);

    float diameter = finalRadius * 2.0f;

    center.x -= diameter * (renderable.origin.x - 0.5f);
    center.y -= diameter * (renderable.origin.y - 0.5f);
    SDL_SetRenderDrawColor(renderer.get(), renderable.color.r,
                           renderable.color.g, renderable.color.b,
                           renderable.color.a);

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
    const Hylozoa::Components::Rendering::Renderable &renderable,
    const Hylozoa::Components::Rendering::RenderableShape &shape,
    const Hylozoa::Components::Camera &camera,
    const Hylozoa::Components::WorldTransform &cameraTransform) {

    std::shared_ptr<SDL_Renderer> &renderer =
        Hylozoa::SDL::SDL_Manager::getInstance().getRenderer();

    const auto &rectSpecs = std::get<
        Hylozoa::Components::Rendering::RenderableShape::RectangleSpecs>(
        shape.specs);

    SDL_FRect fillRect;
    glm::vec2 screenPos =
        worldToView(transform.position, camera, cameraTransform);

    float zoom = camera.isUI ? 1.0f : camera.zoom;
    fillRect.w = rectSpecs.width * transform.scale.x * zoom;
    fillRect.h = rectSpecs.height * transform.scale.y * zoom;
    fillRect.y = screenPos.y - (fillRect.h * renderable.origin.y);
    fillRect.x = screenPos.x - (fillRect.w * renderable.origin.x);

    SDL_SetRenderDrawColor(renderer.get(), renderable.color.r,
                           renderable.color.g, renderable.color.b,
                           renderable.color.a);
    SDL_RenderFillRect(renderer.get(), &fillRect);
}

void Renderer::updateTexture(
    const Hylozoa::Components::WorldTransform &transform,
    const Hylozoa::Components::Rendering::Renderable &renderable,
    const Hylozoa::Components::Rendering::Sprite &sprite,
    Hylozoa::Components::HylozoaInternal::RenderTexture &texture,
    const Hylozoa::Components::Camera &camera,
    const Hylozoa::Components::WorldTransform &cameraTransform) {
    if (texture.texture.expired()) {
        try {
            auto &textureManager = _registry.ctx().get<TextureManager>();
            auto loadedTexture = textureManager.load(
                Hylozoa::Resources::Texture::loader, sprite.textureName);
            // if (loadedTexture == nullptr) {
            //     std::cerr << "Texture '" << sprite.textureName
            //               << "' not found in TextureManager.\n";
            //     return;
            // }
            texture.texture = loadedTexture;
        } catch (const std::exception &e) {
            std::cerr << "Failed to load texture '" << sprite.textureName
                      << "': " << e.what() << "\n";
            return;
        }
    }

    SDL_FPoint texSize = texture.texture.lock()->getSize();

    SDL_FRect localRect{0, 0, texSize.x, texSize.y};

    localRect.w *= transform.scale.x * sprite.scale.x;
    localRect.h *= transform.scale.y * sprite.scale.y;

    glm::vec2 screenPos =
        worldToView(transform.position, camera, cameraTransform);
    localRect.x = screenPos.x - (localRect.w * renderable.origin.x);
    localRect.y = screenPos.y - (localRect.h * renderable.origin.y);
    texture.destRect = localRect;
}

void Renderer::renderTexture(
    const Hylozoa::Components::WorldTransform &transform,
    const Hylozoa::Components::Rendering::Renderable &renderable,
    const Hylozoa::Components::Rendering::Sprite &sprite,
    Hylozoa::Components::HylozoaInternal::RenderTexture &texture,
    const Hylozoa::Components::Camera &camera,
    const Hylozoa::Components::WorldTransform &cameraTransform) {

    SDL_FRect destRect;
    texture.getRect(destRect);

    SDL_Texture *sdlTexture = texture.getTexture();
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
    glm::vec2 cameraCenter = cameraTransform.position + camera.offset;
    glm::vec2 centered = (worldPos - cameraCenter) * camera.zoom;
    glm::vec2 screen = centered + camera.viewportSize * 0.5f;
    return screen;
}

} // namespace Hylozoa::Systems
