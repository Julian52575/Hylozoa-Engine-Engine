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

int main(int ac, char *const *av) {

  Hylozoa::Placeholder pl;

  std::cout << "Hello world from Hylozoa Game Engine main." << std::endl;
  pl.helloWorld();

  Hylozoa::Engine engine;

  auto e1 = engine.getWorld().entity()
      .set<Hylozoa::Transform2D>({0.0f, 0.0f});
  
  std::cout << e1.get<Hylozoa::Transform2D>().x << ", "
            << e1.get<Hylozoa::Transform2D>().y << std::endl;
  
  engine.runTick(2);
  std::cout << e1.get<Hylozoa::Transform2D>().x << ", "
            << e1.get<Hylozoa::Transform2D>().y << std::endl;

  e1.remove<Hylozoa::Transform2D>();
  e1.remove<Hylozoa::Transform2D>();

  
  return 0;
}
