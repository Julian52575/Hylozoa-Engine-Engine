#pragma once

#include <SDL3/SDL.h>
#include <memory>

namespace Hylozoa::SDL {

#define RENDERER_WINDOW_WIDTH 1920
#define RENDERER_WINDOW_HEIGHT 1080

// Singletone of SDL Renderer for the engine
class SDL_Manager {
public:
  static SDL_Manager &getInstance() {
    static SDL_Manager instance;

    return instance;
  }
  std::shared_ptr<SDL_Renderer> &getRenderer() { return _renderer; }
  SDL_Window *getWindow() { return _window; }

private:
  SDL_Manager();
  ~SDL_Manager();

  SDL_Window *_window{nullptr};
  std::shared_ptr<SDL_Renderer> _renderer;
  // Prevent copy and assignment
  SDL_Manager(const SDL_Manager &) = delete;
  SDL_Manager &operator=(const SDL_Manager &) = delete;
};

} // namespace Hylozoa::SDL