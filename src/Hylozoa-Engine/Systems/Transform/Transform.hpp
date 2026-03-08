/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Transform related System [header]
*/

#ifndef TRANSFORM_SYSTEM_HPP_
#define TRANSFORM_SYSTEM_HPP_

#include "Hylozoa-Engine/Systems/Manager/Systems.hpp"
#include <iostream>

namespace Hylozoa {
namespace Systems {
void parent_child_system(entt::registry &registry);
void local_to_world_system(entt::registry &registry);

class ParentChildSystem : public System {
  public:
    ParentChildSystem(entt::registry &registry) : System(registry) {}
    const std::string &getName() const override { return this->_name; }

    void onStart() override { std::cout << "[" << this->_name << "] Start\n"; }

    void onUpdate(float dt) override {
          parent_child_system(this->_registry);
          // std::cout << "[" << this->_name << "] Update frame (" << dt <<
          // "s)\n";
    }

    void onEnd() override { std::cout << "[" << this->_name << "] End\n"; }

  private:
    std::string _name = "ParentChildSystem";
};

class UpdateTransformSystem : public System {
  public:
    UpdateTransformSystem(entt::registry &registry) : System(registry) {}
    const std::string &getName() const override { return this->_name; }

    void onStart() override { std::cout << "[" << this->_name << "] Start\n"; }

    void onUpdate(float dt) override {
          local_to_world_system(this->_registry);
          // std::cout << "[" << this->_name << "] Update frame (" << dt <<
          // "s)\n";
    }

    void onEnd() override { std::cout << "[" << this->_name << "] End\n"; }

  private:
    std::string _name = "UpdateTransformSystem";
};
} // namespace Systems
} // namespace Hylozoa

#endif