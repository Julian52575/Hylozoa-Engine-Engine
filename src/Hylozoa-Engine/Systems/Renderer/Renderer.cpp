// Renderer.cpp
#include "Renderer.hpp"
#include "Hylozoa-Engine/Components/Physics/Physics.hpp"
#include "Hylozoa-Engine/Components/Scene/Scene.hpp"
#include "Hylozoa-Engine/Core/Settings.hpp"
#include "Hylozoa-Engine/Core/Entities/Entity.hpp"
#include <algorithm>
#include <glm/vec2.hpp>
#include <iostream>
#include <vector>

namespace {

enum class RenderCommandType { Shape, Texture };

struct RenderCommand {
    entt::entity entity;
    int zIndex;
    std::size_t sequence;
    RenderCommandType type;
};

glm::vec2 rotatePoint(const glm::vec2& p, float angle)
{
    float c = std::cos(angle);
    float s = std::sin(angle);

    return {
        p.x * c - p.y * s,
        p.x * s + p.y * c
    };
}

void renderQuad(SDL_Renderer* renderer,
    glm::vec2 center,
    float width,
    float height,
    glm::vec2 origin,
    float rotation,
    Hylozoa::Components::Color color)
{
    float ox = origin.x * width;
    float oy = origin.y * height;

    glm::vec2 local[4] =
    {
        {-ox, -oy},
        {width - ox, -oy},
        {width - ox, height - oy},
        {-ox, height - oy}
    };

    SDL_Vertex vertices[4];

    for (int i = 0; i < 4; ++i)
    {
        glm::vec2 p = rotatePoint(local[i], rotation) + center;

        vertices[i].position = { p.x, p.y };
        vertices[i].color.r = color.r / 255.0f;
        vertices[i].color.g = color.g / 255.0f;
        vertices[i].color.b = color.b / 255.0f;
        vertices[i].color.a = color.a / 255.0f;
    }

    constexpr int indices[] =
    {
        0,1,2,
        0,2,3
    };

    SDL_RenderGeometry(renderer, nullptr, vertices, 4, indices, 6);
}

} // namespace

namespace Hylozoa::Systems {

void Renderer::onStart() {
    if (Hylozoa::Settings::getInstance().getSettings().verbose) {
        std::cout << "[" << this->_name << "] Start\n";
    }
    auto& sdlManager = Hylozoa::SDL::SDL_Manager::getInstance();
    auto& settings = Hylozoa::Settings::getInstance().getSettings();

    int windowWidth, windowHeight;
    SDL_GetCurrentRenderOutputSize(sdlManager.getRenderer().get(), &windowWidth, &windowHeight);
    _lightBuffer = SDL_CreateTexture(sdlManager.getRenderer().get(),
    SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
    windowWidth, windowHeight);
    if (!_lightBuffer) {
        SDL_Log("Failed to create light buffer: %s", SDL_GetError());
        return;
    }
    SDL_SetTextureBlendMode(_lightBuffer, SDL_BLENDMODE_MOD);
    SDL_SetTextureAlphaMod(_lightBuffer, 255);
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
            std::cout << "[" << this->_name
                      << "] Warning: No camera found in the scene. Nothing to "
                         "render.\n";
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

void Renderer::onEnd() {
    SDL_DestroyTexture(_lightBuffer);
    std::cout << "[" << this->_name << "] End\n";
}

void Renderer::renderPointLight(
    std::shared_ptr<SDL_Renderer>& renderer,
    glm::vec2 center, float radius,
    const Hylozoa::Components::Rendering::PointLight& light)
{
    const int SEGMENTS = 48;

    std::vector<SDL_Vertex> vertices;
    std::vector<int> indices;

    float intensity = std::clamp(light.intensity, 0.0f, 1.0f);

    SDL_FColor centerColor {
        (light.color.r / 255.f) * intensity,
        (light.color.g / 255.f) * intensity,
        (light.color.b / 255.f) * intensity,
        1.0f
    };

    SDL_FColor edgeColor {
        0,0,0,
        1.0f
    };

    vertices.push_back({{center.x, center.y}, centerColor, {0.5f, 0.5f}});

    for (int i = 0; i <= SEGMENTS; ++i) {
        float angle = i * 2.0f * M_PI / SEGMENTS;
        float vx = center.x + cosf(angle) * radius;
        float vy = center.y + sinf(angle) * radius;
        vertices.push_back({{vx, vy}, edgeColor, {0, 0}});

        if (i > 0) {
            indices.push_back(0);
            indices.push_back(i);
            indices.push_back(i + 1);
        }
    }
    SDL_BlendMode prevMode;
    SDL_GetRenderDrawBlendMode(renderer.get(), &prevMode);

    SDL_SetRenderDrawBlendMode(renderer.get(), SDL_BLENDMODE_ADD);
    SDL_RenderGeometry(renderer.get(), nullptr,
        vertices.data(), (int)vertices.size(),
        indices.data(),  (int)indices.size());
    SDL_SetRenderDrawBlendMode(renderer.get(), prevMode);
}

void Renderer::renderLightPass(
    const Hylozoa::Components::Camera& camera,
    const Hylozoa::Components::WorldTransform& cameraTransform)
{
    auto& renderer = Hylozoa::SDL::SDL_Manager::getInstance().getRenderer();
    
    auto ambientView = _registry.view<
    Hylozoa::Components::Rendering::AmbientLight,
    Hylozoa::Components::HylozoaInternal::SceneActiveTag>();

    if (ambientView.begin() == ambientView.end())
        return;

    const auto& ambient = ambientView.get<
        Hylozoa::Components::Rendering::AmbientLight>(*ambientView.begin());

    SDL_SetRenderTarget(renderer.get(), _lightBuffer);

    float intensity = std::clamp(ambient.intensity, 0.0f, 1.0f);

    SDL_SetRenderDrawColor(renderer.get(),
        ambient.color.r * intensity, ambient.color.g * intensity,
        ambient.color.b * intensity, 255);
    SDL_RenderClear(renderer.get());

    auto lightView = _registry.view<
        Hylozoa::Components::Rendering::PointLight,
        Hylozoa::Components::WorldTransform,
        Hylozoa::Components::HylozoaInternal::SceneActiveTag>();

    for (auto entity : lightView) {
        const auto& light = lightView.get<Hylozoa::Components::Rendering::PointLight>(entity);
        const auto& transform = lightView.get<Hylozoa::Components::WorldTransform>(entity);

        glm::vec2 center = worldToView(transform.position, camera, cameraTransform);
        float screenRadius = light.radius * camera.zoom;

        renderPointLight(renderer, center, screenRadius, light);
    }

    SDL_SetRenderTarget(renderer.get(), nullptr);
    SDL_RenderTexture(renderer.get(), _lightBuffer, nullptr, nullptr);
}

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

    std::vector<RenderCommand> commands;
    std::size_t sequence = 0;

    auto shapeView =
        this->_registry
            .view<Hylozoa::Components::Rendering::Renderable,
                  Hylozoa::Components::Rendering::RenderableShape,
                  Hylozoa::Components::WorldTransform,
                  Hylozoa::Components::HylozoaInternal::SceneActiveTag>();

    for (auto entity : shapeView) {
        const auto &renderable =
            shapeView.get<Hylozoa::Components::Rendering::Renderable>(entity);

        if (!shouldRender(camera, renderable))
            continue;

        commands.push_back(
            {entity, renderable.zIndex, sequence++, RenderCommandType::Shape});
    }

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

        if (!shouldRender(camera, renderable))
            continue;

        commands.push_back(
            {entity, renderable.zIndex, sequence++, RenderCommandType::Texture});
    }

    std::stable_sort(commands.begin(), commands.end(),
                     [](const RenderCommand &a, const RenderCommand &b) {
                         if (a.zIndex == b.zIndex)
                             return a.sequence < b.sequence;
                         return a.zIndex < b.zIndex;
                     });

    for (const auto &command : commands) {
        const auto &renderable =
            this->_registry
                .get<Hylozoa::Components::Rendering::Renderable>(
                    command.entity);
        const auto &transform =
            this->_registry.get<Hylozoa::Components::WorldTransform>(
                command.entity);

        if (command.type == RenderCommandType::Shape) {
            const auto &shape =
                this->_registry
                    .get<Hylozoa::Components::Rendering::RenderableShape>(
                        command.entity);
            renderShape(transform, renderable, shape, camera, cameraTransform);
            continue;
        }

        const auto &sprite =
            this->_registry.get<Hylozoa::Components::Rendering::Sprite>(
                command.entity);
        auto &texture =
            this->_registry
                .get<Hylozoa::Components::HylozoaInternal::RenderTexture>(
                    command.entity);
        updateTexture(transform, renderable, sprite, texture, camera,
                      cameraTransform);
        renderTexture(transform, renderable, sprite, texture, camera,
                      cameraTransform);
    }
    renderLightPass(camera, cameraTransform);
}

bool Renderer::shouldRender(
    const Hylozoa::Components::Camera &camera,
    const Hylozoa::Components::Rendering::Renderable &renderable) const {
    return renderable.visible && camera.cullingMask.contains(renderable.layer);
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

    float scaledThickness = shape.outlineThickness * camera.zoom;
    float innerRadius = finalRadius - scaledThickness;

    const int SEGMENTS = 36;
    std::vector<SDL_Vertex> vertices;
    std::vector<int> indices;

    SDL_FColor outColor = {shape.outlineColor.r / 255.0f, shape.outlineColor.g / 255.0f, shape.outlineColor.b / 255.0f, shape.outlineColor.a / 255.0f};
    SDL_FColor coreColor = {renderable.color.r / 255.0f, renderable.color.g / 255.0f, renderable.color.b / 255.0f, renderable.color.a / 255.0f};

    if (innerRadius > 0.0f) {
        vertices.push_back({{center.x, center.y}, coreColor, {0}});

        for (int i = 0; i <= SEGMENTS; ++i) {
            float angle = i * 2.0f * M_PI / SEGMENTS;
            float vx = center.x + cosf(angle) * innerRadius;
            float vy = center.y + sinf(angle) * innerRadius;
            vertices.push_back({{vx, vy}, coreColor, {0}});

            if (i > 0) {
                indices.push_back(0);
                indices.push_back(i);
                indices.push_back(i + 1);
            }
        }
        SDL_RenderGeometry(renderer.get(), nullptr, vertices.data(), vertices.size(), indices.data(), indices.size());
    }

    vertices.clear();
    indices.clear();

    for (int i = 0; i <= SEGMENTS; ++i) {
        float angle = i * 2.0f * M_PI / SEGMENTS;
        float c = cosf(angle);
        float s = sinf(angle);

        vertices.push_back({{center.x + c * finalRadius, center.y + s * finalRadius}, outColor, {0}});
        vertices.push_back({{center.x + c * std::fmaxf(0.0f, innerRadius), center.y + s * std::fmaxf(0.0f, innerRadius)}, outColor, {0}});

        if (i > 0) {
            int idx = i * 2;
            indices.push_back(idx - 2);
            indices.push_back(idx - 1);
            indices.push_back(idx);
            indices.push_back(idx - 1);
            indices.push_back(idx + 1);
            indices.push_back(idx);
        }
    }
    SDL_RenderGeometry(renderer.get(), nullptr, vertices.data(), vertices.size(), indices.data(), indices.size());
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

    float zoom = camera.isUI ? 1.0f : camera.zoom;

    glm::vec2 screenPos =
        worldToView(transform.position, camera, cameraTransform);

    float angle = glm::radians(transform.rotation);

    float w = rectSpecs.width * transform.scale.x * zoom;
    float h = rectSpecs.height * transform.scale.y * zoom;

    float ox = renderable.origin.x;
    float oy = renderable.origin.y;

    renderQuad(
        renderer.get(),
        screenPos,
        w,
        h,
        {ox, oy},
        angle,
        shape.outlineColor);
    
    w -= shape.outlineThickness * zoom;
    h -= shape.outlineThickness * zoom;

    renderQuad(
        renderer.get(),
        screenPos,
        w,
        h,
        {ox, oy},
        angle,
        renderable.color);
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

    SDL_FPoint center{
        destRect.w * renderable.origin.x,
        destRect.h * renderable.origin.y
    };

    if (!SDL_RenderTextureRotated(
        renderer.get(),
        sdlTexture,
        nullptr,
        &destRect,
        transform.rotation,
        &center,
        SDL_FLIP_NONE)) {
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
