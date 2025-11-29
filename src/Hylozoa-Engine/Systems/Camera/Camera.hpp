/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Camera System [header]
*/

#ifndef CAMERA_SYSTEM_HPP_
#define CAMERA_SYSTEM_HPP_

#include "Hylozoa-Engine/Systems/Manager/Systems.hpp"
#include <iostream>

namespace Hylozoa {

class CameraSystem : public System {
public:
  const std::string &getName() const override { return this->_name; }

  void onStart() override {
    // Initialization code for the camera system
    std::cout << "[" << this->_name << "] Start\n";
  }

  void onUpdate(float dt) override;

  void onEnd() override {
    // Cleanup code for the camera system
    std::cout << "[" << this->_name << "] End\n";
  }

private:
  std::string _name = "CameraSystem";
};

} // namespace Hylozoa

#endif /* !CAMERA_SYSTEM_HPP_ */