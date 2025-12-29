//
// EPITECH PROJECT, 2025
// Hylozoa-Engine
// File description:
// main
//

#include "Hylozoa-Engine/Components/Camera/Camera.hpp"
#include "Hylozoa-Engine/Components/Input/Controllable.hpp"
#include "Hylozoa-Engine/Components/Physics/Physics.hpp"
#include "Hylozoa-Engine/Components/Rendering/Renderable.hpp"
#include "Hylozoa-Engine/Components/Transform/Transform.hpp"
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

  auto ground = engine.scene().spawnEntity("Ground");
  auto player = engine.scene().spawnEntity("Player");
  auto camera = engine.scene().spawnEntity("Main Camera");
  auto camera2 = engine.scene().spawnEntity("Secondary Camera");
  camera.addComponent<Hylozoa::Components::Camera>();

  auto &circle =
      player.addComponent<Hylozoa::Components::CircleColliderComponent>();
  circle.radius = 100.f;

  renderable.color = {0, 0, 255, 255};

  player.getComponent<Hylozoa::Components::LocalTransform>().position = {0.0f,
                                                                         0.0f};
  player.addComponent<Hylozoa::Components::RigidBodyComponent>().type =
      b2_dynamicBody;
  player.addComponent<Hylozoa::Components::ColliderComponent>()
      .enableContactEvents = true;
  player.addComponent<Hylozoa::Components::Rendering::Renderable>(renderable);
  player.addComponent<Hylozoa::Components::Rendering::RenderableShape>(
      Hylozoa::Components::Rendering::RenderableShape{
          Hylozoa::Components::Rendering::RenderableShape::ShapeType::Circle,
          Hylozoa::Components::Rendering::RenderableShape::CircleSpecs{
              circle.radius}});
  player.addComponent<Hylozoa::Components::Controllable>();

  auto &boxGround =
      ground.addComponent<Hylozoa::Components::BoxColliderComponent>();
  boxGround.halfWidth = 500.f;
  boxGround.halfHeight = 50.f;

  renderable.color = {0, 255, 0, 255};
  ground.getComponent<Hylozoa::Components::LocalTransform>().position = {
      0.0f, 300.0f};
  ground.addComponent<Hylozoa::Components::RigidBodyComponent>().type =
      b2_staticBody;
  ground.addComponent<Hylozoa::Components::ColliderComponent>()
      .enableContactEvents = true;
  ground.addComponent<Hylozoa::Components::Rendering::Renderable>(renderable);
  ground.addComponent<Hylozoa::Components::Rendering::RenderableShape>(
      Hylozoa::Components::Rendering::RenderableShape{
          Hylozoa::Components::Rendering::RenderableShape::ShapeType::Rectangle,
          Hylozoa::Components::Rendering::RenderableShape::RectangleSpecs{
              boxGround.halfWidth * 2, boxGround.halfHeight * 2}});

  std::cout << "Player entity: " << player.getName(engine) << std::endl;

  // engine.runTick(90);
  //   engine.runTick(1);
  engine.run();
  return 0;
}
