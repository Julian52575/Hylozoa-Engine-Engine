#pragma once

#include <SDL3/SDL.h>
#include <entt/entt.hpp>

#include <memory>

#include "Hylozoa-Engine/Components/Camera/Camera.hpp"
#include "Hylozoa-Engine/Components/Rendering/Renderable.hpp"
#include "Hylozoa-Engine/Components/Transform/Transform.hpp"
#include "Hylozoa-Engine/SDL/SDL_Manager.hpp"
#include "Hylozoa-Engine/Systems/Manager/Systems.hpp"

namespace Hylozoa::Systems {

/**
 * Texture component - for rendering textured sprites
 * Used by: Entities with textures (will be refactored to use resource manager)
 */
class Renderer : public System {
  public:
    Renderer();
    ~Renderer() = default;
    void onStart() override;
    void onUpdate(float deltaTime) override;
    void onEnd() override;
    const std::string &getName() const override { return this->_name; }

  protected:
    int _priority = 99; // Low priority to run after most systems
    std::string _name = "Renderer";

  private:
    SDL_FRect fillRect = {0, 0, 0, 0};

    void renderSingleCamera(const Components::Camera &camera,
                            const Components::WorldTransform &cameraTransform);

    void renderShape(const Components::WorldTransform &transform,
                     const Components::Rendering::Renderable &renderable,
                     const Components::Rendering::RenderableShape &shape,
                     const Components::Camera &camera,
                     const Components::WorldTransform &cameraTransform);

    void renderShapeCircle(const Components::WorldTransform &transform,
                           const Components::Rendering::Renderable &sprite,
                           const Components::Rendering::RenderableShape &shape,
                           const Components::Camera &camera,
                           const Components::WorldTransform &cameraTransform);

    void
    renderShapeRectangle(const Components::WorldTransform &transform,
                         const Components::Rendering::Renderable &sprite,
                         const Components::Rendering::RenderableShape &shape,
                         const Components::Camera &camera,
                         const Components::WorldTransform &cameraTransform);

    void renderTexture(const Components::WorldTransform &transform,
                       const Components::Rendering::Renderable &renderable,
                       Components::Rendering::RenderableTexture &texture,
                       const Components::Camera &camera,
                       const Components::WorldTransform &cameraTransform);

    inline glm::vec2
    worldToView(const glm::vec2 &worldPos, const Components::Camera &camera,
                const Components::WorldTransform &cameraTransform);
};

} // namespace Hylozoa::Systems