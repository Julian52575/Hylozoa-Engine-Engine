//
// EPITECH PROJECT, 2025
// Hylozoa-Engine
// File description:
// main
//

#include "Hylozoa-Engine/Components/Input/Controllable.hpp"
#include "Hylozoa-Engine/Components/Physics/Physics.hpp"
#include "Hylozoa-Engine/Components/Rendering/Renderable.hpp"
#include "Hylozoa-Engine/Components/Transform/Transform.hpp"
#include "Hylozoa-Engine/Components/Camera/Camera.hpp"
#include "Hylozoa-Engine/Core/Engine.hpp"
#include "Hylozoa-Engine/Core/Entity.hpp"
#include "Hylozoa-Engine/Placeholder/Placeholder.hpp"
#include "Hylozoa-Engine/Systems/HelloWorld/HelloWorld.hpp"
#include <SDL3/SDL.h>
#include <entt/entt.hpp>
#include <iostream>

int main(int ac, char *const *av) {

  std::cout << "Hello world from Hylozoa Game Engine main." << std::endl;

  Hylozoa::Engine engine;
  Hylozoa::Components::Rendering::Renderable renderable;
  renderable.color = {255, 255, 255, 0};

  auto player = engine.createSpacialEntity("Player");
  auto camera = engine.createSpacialEntity("Main Camera");
  camera.addComponent<Hylozoa::Components::Camera>();

  auto &box = player.addComponent<Hylozoa::Components::BoxColliderComponent>();
  box.halfWidth = 50.0f;
  box.halfHeight = 50.0f;

  renderable.color = {0, 0, 255, 255};


  player.getComponent<Hylozoa::Components::LocalTransform>().position = {100, 14.0f};
  player.addComponent<Hylozoa::Components::RigidBodyComponent>().type =
      b2_dynamicBody;
  player.addComponent<Hylozoa::Components::ColliderComponent>()
      .enableContactEvents = true;
player.addComponent<Hylozoa::Components::Rendering::Renderable>(
    renderable);
    player.addComponent<Hylozoa::Components::Rendering::RenderableShape>(
        Hylozoa::Components::Rendering::RenderableShape{
            Hylozoa::Components::Rendering::RenderableShape::ShapeType::Rectangle,
            Hylozoa::Components::Rendering::RenderableShape::RectangleSpecs{
                box.halfWidth * 2, box.halfHeight * 2}});
  player.addComponent<Hylozoa::Components::Controllable>();


  std::cout << "Player entity: " << player.getName(engine) << std::endl;

  // engine.runTick(90);
//   engine.runTick(1);
  engine.run();
  return 0;
}
