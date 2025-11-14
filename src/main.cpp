//
// EPITECH PROJECT, 2025
// Hylozoa-Engine
// File description:
// main
//

#include "Hylozoa-Engine/Components/Physics/Physics.hpp"
#include "Hylozoa-Engine/Components/Rendering/Renderable.hpp"
#include "Hylozoa-Engine/Components/Transform/Transform.hpp"
#include "Hylozoa-Engine/Components/Input/Controllable.hpp"
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
  auto ground = engine.createSpacialEntity("ground");



  weapon.addComponent<Hylozoa::Components::Rendering::Renderable>(renderable);
  weapon.addComponent<Hylozoa::Components::Rendering::RenderableShape>(
    Hylozoa::Components::Rendering::RenderableShape{
          Hylozoa::Components::Rendering::RenderableShape::ShapeType::Circle,
          Hylozoa::Components::Rendering::RenderableShape::CircleSpecs{50.0f}
    }
  );
  weapon.childOf(player);


  camera.addComponent<Hylozoa::Components::Rendering::Renderable>(renderable);
  camera.addComponent<Hylozoa::Components::Rendering::RenderableTexture>(
    Hylozoa::Components::Rendering::RenderableTexture{
          "assets/textures/missing.png"
    }
  );
  camera.childOf(player);


  
  player.getComponent<Hylozoa::LocalTransform>().position = {100, 14.0f};
  player.addComponent<Hylozoa::Components::RigidBodyComponent>().type = b2_dynamicBody;
  player.addComponent<Hylozoa::Components::ColliderComponent>().enableContactEvents = true;

  auto &box = player.addComponent<Hylozoa::Components::BoxColliderComponent>();
  box.halfWidth = 1.0f;
  box.halfHeight = 1.0f;

  player.addComponent<Hylozoa::Components::Controllable>();

  
  ground.getComponent<Hylozoa::LocalTransform>().position = {100.0f, 1000.0f};
  ground.addComponent<Hylozoa::Components::ColliderComponent>().enableContactEvents = true;
  ground.addComponent<Hylozoa::Components::Rendering::Renderable>(renderable);
  ground.addComponent<Hylozoa::Components::Rendering::RenderableTexture>(
    Hylozoa::Components::Rendering::RenderableTexture{
          "assets/textures/Ground.png"
    }
  );
  
  auto &groundbox = ground.addComponent<Hylozoa::Components::BoxColliderComponent>();
  groundbox.halfWidth = 50.0f;
  groundbox.halfHeight = 10.0f;
  // ground.addComponent<Hylozoa::Components::RigidBodyComponent>();



  std::cout << "Player entity: " << player.getName(engine) << std::endl;
  std::cout << "Camera entity: " << camera.getName(engine) << std::endl;

  // engine.runTick(90);
  // engine.runTick(-1);
  engine.run();
  return 0;
}
