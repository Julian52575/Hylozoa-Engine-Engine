#include "Hylozoa-Engine/Systems/Movement/Movement.hpp"
#include <iostream>
#include <SDL3/SDL.h>

#include "Hylozoa-Engine/Components/Input/Controllable.hpp"
#include "Hylozoa-Engine/Components/Transform/Transform.hpp"

namespace Hylozoa::Systems {
    Movement::Movement() {}

    void Movement::onStart() {
        std::cout << "[" << this->_name << "] Start\n";
    }

    void Movement::onUpdate(float deltaTime) {
        if (this->_registry) {
            for (auto &entity : this->_registry->view<Hylozoa::Components::Controllable,Hylozoa::LocalTransform, Hylozoa::Name>()){
                auto &worldTransform = this->_registry->get<Hylozoa::LocalTransform>(entity);
                auto &controllable = this->_registry->get<Hylozoa::Components::Controllable>(entity);
                auto &name = this->_registry->get<Hylozoa::Name>(entity);

                for (const auto &key : controllable.keysHeld){
                    if (controllable.keysHeld.contains(SDLK_W) || controllable.keysHeld.contains(SDLK_UP))
                        worldTransform.position.y -= 500.0f * deltaTime;
                    if (controllable.keysHeld.contains(SDLK_S) || controllable.keysHeld.contains(SDLK_DOWN))
                        worldTransform.position.y += 500.0f * deltaTime;
                    if (controllable.keysHeld.contains(SDLK_A) || controllable.keysHeld.contains(SDLK_LEFT))
                        worldTransform.position.x -= 500.0f * deltaTime;
                    if (controllable.keysHeld.contains(SDLK_D) || controllable.keysHeld.contains(SDLK_RIGHT))
                        worldTransform.position.x += 500.0f * deltaTime;
                }
                std::cout << "[" << this->_name << "] Entity " << name.name << " Position: (" << worldTransform.position.x << ", " << worldTransform.position.y << ")\n";
            }
        }
    }

    void Movement::onEnd() {
        std::cout << "[" << this->_name << "] End\n";
    }

    const std::string &Movement::getName() const {
        return this->_name;
    }

} // namespace Hylozoa::Systems