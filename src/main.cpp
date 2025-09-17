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
  engine.getWorld().component<Hylozoa::Transform2D>().set_name("Transform2D");

  bool idk = engine.getWorld().script().filename("config/test.flecs").run();
  std::cout << "Config run result: _" << idk << "_" << std::endl;

  // After running the script, you can access your Test entity by its name
  flecs::entity testEntity = engine.getWorld().lookup("Test");
  if (testEntity.is_valid()) {
    // Example: print the entity id
    std::cout << "Test entity id: " << testEntity.id() << std::endl;

    // If you want to access a component, for example Transform:
    if (testEntity.has<Hylozoa::Transform2D>()) {
      auto transform = testEntity.get<Hylozoa::Transform2D>();
      // Use the transform as needed

      std::cout << "Test entity has Transform2D component. " << transform.x << std::endl;
    }
  } else {
    std::cout << "Test entity not found!" << std::endl;
  }
  return 0;
}
