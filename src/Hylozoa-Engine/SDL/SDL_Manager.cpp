#include <SDL3/SDL.h>

#include "SDL_Manager.hpp"

namespace Hylozoa::SDL {

SDL_Manager::SDL_Manager() {
  SDL_Renderer *tmpRenderer = NULL;

  SDL_SetAppMetadata("Hylozoa Engine", "1.0",
                     "...A game engine made with love by Epitech students.");
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

SDL_Manager::~SDL_Manager() {
  SDL_Quit();

  if (_window) {
    SDL_DestroyWindow(_window);
    _window = nullptr;
  }
  _renderer.reset();
}

} // namespace Hylozoa::SDL