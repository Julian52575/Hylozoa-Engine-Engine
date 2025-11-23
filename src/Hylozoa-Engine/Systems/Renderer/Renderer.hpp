#pragma once

#include <SDL3/SDL.h>
#include <entt/entt.hpp>

#include <memory>

#include "Hylozoa-Engine/Components/Rendering/Renderable.hpp"
#include "Hylozoa-Engine/Components/Transform/Transform.hpp"
#include "Hylozoa-Engine/Components/Camera/Camera.hpp"
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

  inline void
  renderShape(const Hylozoa::Components::WorldTransform &,
              const Hylozoa::Components::Rendering::Renderable &,
              const Hylozoa::Components::Rendering::RenderableShape &);
  inline void
  renderShapeCircle(const Hylozoa::Components::WorldTransform &,
                    const Hylozoa::Components::Rendering::Renderable &,
                    const Hylozoa::Components::Rendering::RenderableShape &);
  inline void
  renderShapeRectangle(const Hylozoa::Components::WorldTransform &,
                       const Hylozoa::Components::Rendering::Renderable &,
                       const Hylozoa::Components::Rendering::RenderableShape &);
  inline void
  renderTexture(const Hylozoa::Components::WorldTransform &,
                const Hylozoa::Components::Rendering::Renderable &,
                Hylozoa::Components::Rendering::RenderableTexture &);

  inline SDL_FRect worldToScreen(const Components::WorldTransform &objTransform,
                                const Components::WorldTransform &cameraTransform,
                                const Components::Camera &camera);
};

} // namespace Hylozoa::Systems