#include <SDL3/SDL.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#include "SDL_Manager.hpp"

#include <fstream>


namespace Hylozoa::SDL {

SDL_Manager::SDL_Manager() {
  SDL_Renderer *tmpRenderer = NULL;

  SDL_SetAppMetadata("Hylozoa Engine", "1.0","...A game engine made with love by Epitech students.");

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
    return;
  }
  this->_window = SDL_CreateWindow("Hylozoa Engine",RENDERER_WINDOW_WIDTH,RENDERER_WINDOW_HEIGHT,SDL_WINDOW_RESIZABLE);
  if (!this->_window) {
    SDL_Log("Couldn't create window: %s", SDL_GetError());
    return;
  }
  this->_bgfx_manager.initialize(this->_window);
}


SDL_Manager::~SDL_Manager() {
  this->_bgfx_manager.terminate();
  if (_window) {
    SDL_DestroyWindow(_window);
    _window = nullptr;
  }
  SDL_Quit();
  _renderer.reset();
}

} // namespace Hylozoa::SDL