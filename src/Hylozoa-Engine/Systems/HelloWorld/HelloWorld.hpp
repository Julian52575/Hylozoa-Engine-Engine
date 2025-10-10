/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Hello World system (temporary)
*/

#ifndef HELLOWORLD_HPP_
#define HELLOWORLD_HPP_
#include <flecs.h>
#include <iostream>

namespace Hylozoa {
class HelloWorld {
public:
  HelloWorld(flecs::world &world) { // System declaration
    sys = world.system("Hello World").each([](void) {
      std::cout << "Hello World from Flecs ECS." << std::endl;
    });
  };
  ~HelloWorld() = default;
  void run() { sys.run(); };

protected:
private:
  flecs::system sys;
};
} // namespace Hylozoa

#endif /* !HELLOWORLD_HPP_ */
