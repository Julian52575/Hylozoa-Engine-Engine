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

  auto weapon = engine.createSpacialEntity("Weapon");
  auto camera = engine.createSpacialEntity("Camera");
  auto player = engine.createSpacialEntity("Player");
  auto playerSprite = engine.createSpacialEntity("PlayerSprite");
  auto ground = engine.createSpacialEntity("ground");
  auto groundSprite = engine.createSpacialEntity("groundSprite");
  auto wall = engine.createSpacialEntity("wall");
  auto wallSprite = engine.createSpacialEntity("wallSprite");

  weapon.addComponent<Hylozoa::Components::Rendering::Renderable>(renderable);
  weapon.addComponent<Hylozoa::Components::Rendering::RenderableShape>(
      Hylozoa::Components::Rendering::RenderableShape{
          Hylozoa::Components::Rendering::RenderableShape::ShapeType::Circle,
          Hylozoa::Components::Rendering::RenderableShape::CircleSpecs{30.0f}});
  weapon.childOf(player);
  weapon.getComponent<Hylozoa::LocalTransform>().position = {15.0f, 50 - 15};

  camera.addComponent<Hylozoa::Components::Rendering::Renderable>(renderable);
  camera.addComponent<Hylozoa::Components::Rendering::RenderableTexture>(
      Hylozoa::Components::Rendering::RenderableTexture{
          "assets/textures/camera.png"});
  camera.childOf(player);

  auto &box = player.addComponent<Hylozoa::Components::BoxColliderComponent>();
  box.halfWidth = 50.0f;
  box.halfHeight = 50.0f;

  // Using a child entity for player sprite as a workarround for BoxCollider and
  // RenderableShape conflict assertion error
  renderable.color = {0, 0, 255, 255};
  playerSprite.addComponent<Hylozoa::Components::Rendering::Renderable>(
      renderable);
  playerSprite.addComponent<Hylozoa::Components::Rendering::RenderableShape>(
      Hylozoa::Components::Rendering::RenderableShape{
          Hylozoa::Components::Rendering::RenderableShape::ShapeType::Rectangle,
          Hylozoa::Components::Rendering::RenderableShape::RectangleSpecs{
              box.halfWidth * 2, box.halfHeight * 2}});
  playerSprite.childOf(player);

  player.getComponent<Hylozoa::LocalTransform>().position = {100, 14.0f};
  player.addComponent<Hylozoa::Components::RigidBodyComponent>().type =
      b2_dynamicBody;
  player.addComponent<Hylozoa::Components::ColliderComponent>()
      .enableContactEvents = true;
  player.addComponent<Hylozoa::Components::Controllable>();

  auto &groundPos = ground.getComponent<Hylozoa::LocalTransform>().position = {
      100.0f, 120.0f};
  ground.addComponent<Hylozoa::Components::ColliderComponent>()
      .enableContactEvents = true;

  auto &groundbox =
      ground.addComponent<Hylozoa::Components::BoxColliderComponent>();
  groundbox.halfWidth = 500.0f;
  groundbox.halfHeight = 30.0f;
  ground.addComponent<Hylozoa::Components::RigidBodyComponent>();

  groundSprite.getComponent<Hylozoa::LocalTransform>().position = {0, 20};
  renderable.color = {0, 255, 0, 255};
  groundSprite.addComponent<Hylozoa::Components::Rendering::Renderable>(
      renderable);
  groundSprite.addComponent<Hylozoa::Components::Rendering::RenderableShape>(
      Hylozoa::Components::Rendering::RenderableShape{
          Hylozoa::Components::Rendering::RenderableShape::ShapeType::Rectangle,
          Hylozoa::Components::Rendering::RenderableShape::RectangleSpecs{
              groundbox.halfWidth * 2, groundbox.halfHeight * 2}});
  groundSprite.childOf(ground);

  wall.getComponent<Hylozoa::LocalTransform>().position = {400.0f, 20.0f};
  wall.addComponent<Hylozoa::Components::ColliderComponent>()
      .enableContactEvents = true;
  wall.addComponent<Hylozoa::Components::RigidBodyComponent>();
  auto &wallbox =
      wall.addComponent<Hylozoa::Components::BoxColliderComponent>();
  wallbox.halfWidth = 30.0f;
  wallbox.halfHeight = 200.0f;
  wallSprite.getComponent<Hylozoa::LocalTransform>().position = {0, 20};
  wallSprite.addComponent<Hylozoa::Components::Rendering::Renderable>(
      renderable);
  wallSprite.addComponent<Hylozoa::Components::Rendering::RenderableShape>(
      Hylozoa::Components::Rendering::RenderableShape{
          Hylozoa::Components::Rendering::RenderableShape::ShapeType::Rectangle,
          Hylozoa::Components::Rendering::RenderableShape::RectangleSpecs{
              wallbox.halfWidth * 2, wallbox.halfHeight * 2}});
  wallSprite.childOf(wall);

  auto sprite = engine.createSpacialEntity("sprite");
  sprite.getComponent<Hylozoa::LocalTransform>().position = {300.0f, 300.0f};
  sprite.addComponent<Hylozoa::Components::Rendering::Renderable>(renderable);
  sprite.addComponent<Hylozoa::Components::Rendering::RenderableTexture>(
      Hylozoa::Components::Rendering::RenderableTexture{
          "assets/textures/Ground.png"});

  std::cout << "Player entity: " << player.getName(engine) << std::endl;
  std::cout << "Camera entity: " << camera.getName(engine) << std::endl;

  // engine.runTick(90);
  // engine.runTick(-1);
  engine.run();
  return 0;
}
