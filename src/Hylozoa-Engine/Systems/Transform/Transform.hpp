/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Transform related System [header]
*/

#ifndef MOVEMENTS_HPP_
#define MOVEMENTS_HPP_

#include "Hylozoa-Engine/Systems/Manager/Systems.hpp"
#include <iostream>

namespace Hylozoa {
void parent_child_system(entt::registry &registry);
void local_to_world_system(entt::registry &registry);

class ParentChildSystem : public System {
public:
  const std::string &getName() const override { return this->_name; }

  void onStart() override { std::cout << "[" << this->_name << "] Start\n"; }

  void onUpdate(float dt) override {
    if (this->_registry) {
      parent_child_system(*this->_registry);
      std::cout << "[" << this->_name << "] Update frame (" << dt << "s)\n";
    }
  }

  void onEnd() override { std::cout << "[" << this->_name << "] End\n"; }

private:
  std::string _name = "ParentChildSystem";
};

class UpdateTransformSystem : public System {
public:
  const std::string &getName() const override { return this->_name; }

  void onStart() override { std::cout << "[" << this->_name << "] Start\n"; }

  void onUpdate(float dt) override {
    if (this->_registry) {
      local_to_world_system(*this->_registry);
      std::cout << "[" << this->_name << "] Update frame (" << dt << "s)\n";
    }
  }

  void onEnd() override { std::cout << "[" << this->_name << "] End\n"; }

private:
  std::string _name = "UpdateTransformSystem";
};
} // namespace Hylozoa

#endif