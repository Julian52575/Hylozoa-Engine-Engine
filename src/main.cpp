//
// EPITECH PROJECT, 2025
// Hylozoa-Engine
// File description:
// main
//

#include "Hylozoa-Engine/Components/Transform.hpp"
#include "Hylozoa-Engine/Core/Engine.hpp"
#include "Hylozoa-Engine/Placeholder/Placeholder.hpp"
#include "Hylozoa-Engine/Systems/HelloWorld/HelloWorld.hpp"
#include <SDL3/SDL.h>
#include <entt/entt.hpp>
#include <iostream>


#include "Hylozoa-Engine/Systems/RenderSystem.hpp"
#include "Hylozoa-Engine/Systems/SystemManager.hpp"
#include "Hylozoa-Engine/Systems/Core/TransformSystem.hpp"

class SDL3 {
public:
  SDL3(const char *title, int width, int height) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
      throw std::runtime_error("SDL3 error: Init: " +
                               std::string(SDL_GetError()));
    }
    _window = SDL_CreateWindow(title, width, height,
                               SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!_window) {
      SDL_Quit();
      throw std::runtime_error("SDL3 error: CreateWindow: " +
                               std::string(SDL_GetError()));
    }
    _renderer = SDL_CreateRenderer(_window, nullptr);
    if (!_renderer) {
      SDL_DestroyWindow(_window);
      SDL_Quit();
      throw std::runtime_error("SDL3 error: CreateRenderer: " +
                               std::string(SDL_GetError()));
    }
  }

  void loop() {
    SDL_FRect carre = {380.0f, 280.0f, 40.0f, 40.0f};
    Uint64 lastTime = SDL_GetTicksNS();

    while (_running) {
      Uint64 currentTime = SDL_GetTicksNS();
      float deltaTime = (currentTime - lastTime) / 1e9f; // Convert to seconds
      lastTime = currentTime;

      while (SDL_PollEvent(&_event)) {
        switch (_event.type) {
        case (SDL_EVENT_QUIT):
          _running = false;
          break;

        case (SDL_EVENT_KEY_DOWN):
          if (_event.key.key == SDLK_ESCAPE) {
            _running = false;
          }
          break;

        default:
          break;
        }
      }
      const bool *state = SDL_GetKeyboardState(NULL);
      if (state[SDL_SCANCODE_UP]) {
        carre.y -= 5.0f * deltaTime; // Move up
      }
      if (state[SDL_SCANCODE_DOWN]) {
        carre.y += 5.0f * deltaTime; // Move down
      }
      if (state[SDL_SCANCODE_LEFT]) {
        carre.x -= 5.0f * deltaTime; // Move left
      }
      if (state[SDL_SCANCODE_RIGHT]) {
        carre.x += 5.0f * deltaTime; // Move right
      }

      SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
      SDL_RenderClear(_renderer);

      SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255);
      SDL_RenderFillRect(_renderer, &carre);

      SDL_RenderPresent(_renderer);
      SDL_Delay(16); // ~60 FPS
    }
  }

  ~SDL3() {
    if (_renderer)
      SDL_DestroyRenderer(_renderer);
    if (_window)
      SDL_DestroyWindow(_window);
    SDL_Quit();
  }

private:
  SDL_Window *_window = nullptr;
  SDL_Renderer *_renderer = nullptr;
  bool _running = true;
  SDL_Event _event;
};

int main(int ac, char *const *av) {

  std::cout << "Hello world from Hylozoa Game Engine main." << std::endl;

  Hylozoa::Engine engine;

  auto child2 = engine.createSpacialEntity("Weapon");
  auto child = engine.createSpacialEntity("Camera");
  auto parent = engine.createSpacialEntity("Player");

  child2.child_of(child, engine);
  child.child_of(parent, engine);

  engine.get_registry()
      .get<Hylozoa::LocalTransform>(parent.get_internal_entity())
      .position = {10, 0};
  engine.get_registry()
      .get<Hylozoa::LocalTransform>(child.get_internal_entity())
      .position = {5, 0};
  engine.get_registry()
      .get<Hylozoa::LocalTransform>(child2.get_internal_entity())
      .position = {2, 0};

  std::cout << "Parent entity: " << parent.get_name(engine) << std::endl;
  std::cout << "Child entity: " << child.get_name(engine) << std::endl;

  engine.runTick(1);

  std::cout << "Child world pos: "
            << engine.get_registry()
                   .get<Hylozoa::HylozoaInternal::LocalToWorld>(
                       child2.get_internal_entity())
                   .matrix[0][2]
            << ", "
            << engine.get_registry()
                   .get<Hylozoa::HylozoaInternal::LocalToWorld>(
                       child2.get_internal_entity())
                   .matrix[1][2]
            << std::endl;




    
    //Ajout d'un composant en castant les valeurs dans le bon type
    auto test = engine.createEntity("TestEntity2");
    engine.addComponentToEntity<Hylozoa::Components::Transform2D>(
      test, Hylozoa::Components::Transform2D{{10.0f, 10.0f}, 45.0f, {2.0f, 2.0f}}
    );

    //Ajout d'un composant en passant directement les valeurs, sans caster
    auto test2 = engine.createEntity("TestEntity");
    engine.addComponentToEntity<Hylozoa::Components::Transform2D>(
      test2, {{0.0f, 0.0f}, 0.0f, {1.0f, 1.0f}}
    );

    //Ajout d'un composant en passant les valeurs directement en arguments, sans création d'un objet temporaire
    auto test3 = engine.createEntity("TestEntity3");
    engine.addComponentToEntity<Hylozoa::Components::Transform2D>(
      test3, std::pair<float,float>{0,0}, 90.0f, std::pair<float,float>{0.5f,0.5f}
    );
    engine.addComponentToEntity<Hylozoa::Components::Velocity2D>(
      test3, std::pair<float,float>{5.0f,3.0f}
    );


    Hylozoa::SystemManager systemManager(engine.get_registry());
    auto renderSystem = systemManager.registerSystem<Hylozoa::RenderSystem>();

    //test2 et test ne seront pas joués car ils n'ont pas de Velocity2D
    auto transformSystem = systemManager.registerSystem<Hylozoa::TransformSystem>();

    systemManager.startAll();
    systemManager.updateAll(0.016f); // Simulate a frame update with 16ms delta time
    systemManager.endAll();

  return 0;
}
