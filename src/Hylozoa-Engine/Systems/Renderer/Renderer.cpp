#include <SDL3/SDL.h>
#include <entt/entt.hpp>

#include <iostream>

#include "Renderer.hpp"

namespace Hylozoa::Systems {

Renderer::Renderer() {
  SDL_Renderer *tmpRenderer = NULL;

  SDL_SetAppMetadata("Example Renderer Textures", "1.0",
                     "com.example.renderer-textures");
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
    return;
  }
  if (!SDL_CreateWindowAndRenderer("examples/renderer/textures",
                                   RENDERER_WINDOW_WIDTH,
                                   RENDERER_WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE,
                                   &_window, &tmpRenderer)) {
    SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
    return;
  }
  SDL_SetRenderLogicalPresentation(tmpRenderer, RENDERER_WINDOW_WIDTH,
                                   RENDERER_WINDOW_HEIGHT,
                                   SDL_LOGICAL_PRESENTATION_LETTERBOX);
  _renderer = std::shared_ptr<SDL_Renderer>(tmpRenderer, SDL_DestroyRenderer);
}

void Renderer::onStart() { std::cout << "[" << this->_name << "] Start\n"; }

void Renderer::onUpdate(float deltaTime) {
  if (not this->_registry) {
    SDL_SetRenderDrawColor(_renderer.get(), 148, 0, 211, 255);
    SDL_RenderClear(_renderer.get());
    SDL_RenderPresent(_renderer.get());
    return;
  }
  SDL_SetRenderDrawColor(_renderer.get(), 0, 0, 0, 255);
  SDL_RenderClear(_renderer.get());
  SDL_RenderPresent(_renderer.get());
}

void Renderer::onEnd() {
  SDL_Quit();
  if (_window) {
    SDL_DestroyWindow(_window);
    _window = nullptr;
  }
  std::cout << "[" << this->_name << "] End\n";
}
} // namespace Hylozoa::Systems