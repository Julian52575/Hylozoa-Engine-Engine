/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Transform related System [header]
*/

#ifndef TRANSFORM_SYSTEM_HPP_
#define TRANSFORM_SYSTEM_HPP_

#include "Hylozoa-Engine/Core/Settings.hpp"
#include "Hylozoa-Engine/Systems/Manager/Systems.hpp"
#include <iostream>

namespace Hylozoa {
namespace Systems {
class ParentChildSystem : public System {
  public:
    ParentChildSystem(entt::registry &registry) : System(registry) {}
    const std::string &getName() const override { return this->_name; }

    void onStart() override {
        if (Hylozoa::Settings::getInstance().getSettings().verbose) {
            std::cout << "[" << this->_name << "] Start\n";
        }
    }

    void onUpdate(float dt) override {
        updateParentChild(this->_registry);
        // std::cout << "[" << this->_name << "] Update frame (" << dt <<
        // "s)\n";
    }

    void onEnd() override {
        if (Hylozoa::Settings::getInstance().getSettings().verbose) {
            std::cout << "[" << this->_name << "] End\n";
        }
    }

  private:
    std::string _name = "ParentChildSystem";

  private:
    void updateParentChild(entt::registry &registry);
};

class UpdateTransformSystem : public System {
  public:
    UpdateTransformSystem(entt::registry &registry) : System(registry) {}
    const std::string &getName() const override { return this->_name; }

    void onStart() override {
        if (Hylozoa::Settings::getInstance().getSettings().verbose) {
            std::cout << "[" << this->_name << "] Start\n";
        }
    }

    void onUpdate(float dt) override {
        updateLocalToWorld(this->_registry);
        // std::cout << "[" << this->_name << "] Update frame (" << dt <<
        // "s)\n";
    }

    void onEnd() override {
        if (Hylozoa::Settings::getInstance().getSettings().verbose) {
            std::cout << "[" << this->_name << "] End\n";
        }
    }

  private:
    std::string _name = "UpdateTransformSystem";

  private:
    void updateLocalToWorld(entt::registry &registry);
};
} // namespace Systems
} // namespace Hylozoa

#endif