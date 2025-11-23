/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Camera System [source]
*/

#include "Camera.hpp"

namespace Hylozoa {

void CameraSystem::onUpdate(float dt) {
    if (this->_registry) {
        // Camera update logic goes here
        // For example, updating the camera position based on player position
        // or handling camera zoom levels.

        // std::cout << "[" << this->_name << "] Update frame (" << dt << "s)\n";
        // Well maybe a dedicated Camera is not even needed.
    }
}

}