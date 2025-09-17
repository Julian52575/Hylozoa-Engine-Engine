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
#include <fstream>
#include <sstream>
#include <string>
#include <cstring> // for memset

inline void runScript(flecs::world &world, const std::string &filename) {
    // 2) Read the script file (use absolute path if needed)
    std::ifstream in(filename);

    if (!in) {
        std::cerr << "Failed to open script file: " << filename << "\n";
        return;
    }
    std::stringstream buf;
    buf << in.rdbuf();
    std::string src = buf.str();  
  
    // 3) Use the C API to run the script and capture evaluation errors
    ecs_script_eval_result_t eval_result;
    memset(&eval_result, 0, sizeof(eval_result));

    // world.c_ptr() gives you the raw ecs_world_t* for passing to C API.
    int rc = ecs_script_run(world.c_ptr(), filename.c_str(), src.c_str(), &eval_result);

    std::cout << "ecs_script_run returned: " << rc << "\n";
    if (eval_result.error) {
        std::cerr << "Script error:\n" << eval_result.error << "\n";
        // free the error string (allocated by Flecs)
        ecs_os_free(eval_result.error);
    }
    return;
}

int main(int ac, char *const *av) {
  Hylozoa::Placeholder pl;

  std::cout << "Hello world from Hylozoa Game Engine main." << std::endl;
  pl.helloWorld();

  Hylozoa::Engine engine;
  engine.getWorld().component<Hylozoa::Transform2D>().set_name("Transform2D");

  runScript(engine.getWorld(), "config/test.flecs");
  //std::cout << "Config run result: _" << testEntity << "_" << std::endl;

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
    } else {
      std::cout << "Test entity does not have Transform2D component." << std::endl;
    }
  } else {
    std::cout << "Test entity not found!" << std::endl;
  }
  return 0;
}
