//
// EPITECH PROJECT, 2025
// Hylozoa-Engine
// File description:
// main
//

#include "Hylozoa-Engine/Placeholder/Placeholder.hpp"
#include "Hylozoa-Engine/Systems/HelloWorld/HelloWorld.hpp"
#include <flecs.h>
#include <iostream>

int main(int ac, char *const *av) {
  flecs::world world;
  Hylozoa::Placeholder pl;
  Hylozoa::HelloWorld helloWorldSystem(world);

  std::cout << "Hello world from Hylozoa Game Engine main." << std::endl;
  pl.helloWorld();
  helloWorldSystem.run();
  return 0;
}
