//
// EPITECH PROJECT, 2025
// Hylozoa-Engine
// File description:
// main
//

#include "Hylozoa-Engine/Placeholder/Placeholder.hpp"
#include "Hylozoa-Engine/Systems/HelloWorld/HelloWorld.hpp"
#include "Hylozoa-Engine/Core/Engine.hpp"
#include "Hylozoa-Engine/Components/Transform.hpp"
#include <flecs.h>
#include <iostream>
#include "Hylozoa-Engine/SDL/SDL.hpp"

int main(int ac, char *const *av) {

  Hylozoa::Placeholder pl;
  std::cout << "Hello world from Hylozoa Game Engine main." << std::endl;
  pl.helloWorld();

  Hylozoa::Engine engine;

  // use of Hylozoa Entity
  auto e0 = engine.createEntity("MyEntity");
  e0.addComponent<Hylozoa::Transform2D>({1.0f, 1.0f});

  
  // use of Flecs entity
  auto e1 = engine.getWorld().entity("name")
      .set<Hylozoa::Transform2D>({0.0f, 0.0f});
  
  std::cout << e1.get<Hylozoa::Transform2D>().x << ", "
            << e1.get<Hylozoa::Transform2D>().y << std::endl;

  std::cout << e0.get<Hylozoa::Transform2D>().x << ", "
            << e0.get<Hylozoa::Transform2D>().y << std::endl;
  
  engine.runTick(2);
  std::cout << e1.get<Hylozoa::Transform2D>().x << ", "
            << e1.get<Hylozoa::Transform2D>().y << std::endl;
  std::cout << e0.get<Hylozoa::Transform2D>().x << ", "
            << e0.get<Hylozoa::Transform2D>().y << std::endl;

  e1.remove<Hylozoa::Transform2D>();
  e0.remove<Hylozoa::Transform2D>();

  auto parent = engine.createEntity("Parent");
  auto child = engine.createEntity("Child").childOf(parent);
  std::cout << "Parent: " << parent.name() << std::endl;
  std::cout << "Child: " << child.name() << std::endl;
  std::cout << "Child's parent: " << child.target(flecs::ChildOf).name() << std::endl;

  e1.destruct();
  e0.destruct();

  return 0;
}
