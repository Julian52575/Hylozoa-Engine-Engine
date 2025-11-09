#pragma once

#include <SDL3/SDL.h>
#include <entt/entt.hpp>

#include <memory>

#include "Hylozoa-Engine/Systems/Manager/Systems.hpp"

namespace Hylozoa::Systems {

#define RENDERER_WINDOW_WIDTH 1920
#define RENDERER_WINDOW_HEIGHT 1080

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
  SDL_Window *_window = nullptr;
  std::shared_ptr<SDL_Renderer> _renderer = nullptr;
  SDL_FRect fillRect = {0, 0, RENDERER_WINDOW_WIDTH, RENDERER_WINDOW_HEIGHT};
};

} // namespace Hylozoa::Systems