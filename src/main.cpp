//
// EPITECH PROJECT, 2025
// Hylozoa-Engine
// File description:
// main
//

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

  auto child2 = engine.createSpacialEntity("Weapon");
  auto child = engine.createSpacialEntity("Camera");
  auto parent = engine.createSpacialEntity("Player");

  child2.childOf(child);
  child.childOf(parent);

  parent.getComponent<Hylozoa::LocalTransform>().position = {0, 4.0f};
  parent.addComponent<Hylozoa::Components::RigidBodyComponent>().type =
      b2_dynamicBody;
  parent.addComponent<Hylozoa::Components::ColliderComponent>()
      .enableContactEvents = true;
  auto &box = parent.addComponent<Hylozoa::Components::BoxColliderComponent>();
  box.width = 1.0f;
  box.height = 1.0f;
  // child.getComponent<Hylozoa::LocalTransform>().position = {5, 0};
  // child2.getComponent<Hylozoa::LocalTransform>().position = {2, 0};

  auto ground = engine.createSpacialEntity("ground");
  ground.getComponent<Hylozoa::LocalTransform>().position = {0, -10.0f};
  ground.addComponent<Hylozoa::Components::RigidBodyComponent>();
  ground.addComponent<Hylozoa::Components::ColliderComponent>()
      .enableContactEvents = true;
  auto &groundbox =
      ground.addComponent<Hylozoa::Components::BoxColliderComponent>();
  groundbox.width = 50.0f;
  groundbox.height = 10.0f;

  // Renderer components
  Hylozoa::Components::Rendering::Renderable renderable;

  renderable.color = {255, 255, 255, 0};
  child.addComponent<Hylozoa::Components::Rendering::Renderable>(renderable);
  child.addComponent<Hylozoa::Components::Rendering::RenderableTexture>(
      Hylozoa::Components::Rendering::RenderableTexture{
          "assets/textures/missing.png"});

  child2.addComponent<Hylozoa::Components::Rendering::Renderable>(renderable);
  child2.addComponent<Hylozoa::Components::Rendering::RenderableShape>(
      Hylozoa::Components::Rendering::RenderableShape{
          Hylozoa::Components::Rendering::RenderableShape::ShapeType::Circle,
          Hylozoa::Components::Rendering::RenderableShape::CircleSpecs{50.0f}});

  std::cout << "Parent entity: " << parent.getName(engine) << std::endl;
  std::cout << "Child entity: " << child.getName(engine) << std::endl;

  engine.runTick(90);
  return 0;
}
