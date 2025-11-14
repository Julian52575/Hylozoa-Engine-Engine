#include "Hylozoa-Engine/Systems/Movement/Movement.hpp"
#include <iostream>
#include <SDL3/SDL.h>

#include "Hylozoa-Engine/Components/Input/Controllable.hpp"
#include "Hylozoa-Engine/Components/Transform/Transform.hpp"
#include "Hylozoa-Engine/Components/Physics/Physics.hpp"

namespace Hylozoa::Systems {
    Movement::Movement() {}

    void Movement::onStart() {
        std::cout << "[" << this->_name << "] Start\n";
    }

    void Movement::onUpdate(float deltaTime) {
        if (this->_registry) {
            for (auto &entity : this->_registry->view<Hylozoa::Components::Controllable,Hylozoa::Components::RigidBodyComponent, Hylozoa::LocalTransform, Hylozoa::Name>()){
                auto &rb = this->_registry->get<Hylozoa::Components::RigidBodyComponent>(entity);
                auto &controllable = this->_registry->get<Hylozoa::Components::Controllable>(entity);
                auto &name = this->_registry->get<Hylozoa::Name>(entity);
                auto &transform = this->_registry->get<Hylozoa::LocalTransform>(entity);

                transform.position.x += rb.linearVelocity.x * deltaTime;
                transform.position.y += rb.linearVelocity.y * deltaTime;

                for (const auto &key : controllable.keysHeld){
                    if (controllable.keysHeld.contains(SDLK_W) || controllable.keysHeld.contains(SDLK_UP))
                        rb.linearVelocity.y = -50.0f;
                    if (controllable.keysHeld.contains(SDLK_S) || controllable.keysHeld.contains(SDLK_DOWN))
                        rb.linearVelocity.y = 50.0f;
                    if (controllable.keysHeld.contains(SDLK_A) || controllable.keysHeld.contains(SDLK_LEFT))
                        rb.linearVelocity.x = -50.0f;
                    if (controllable.keysHeld.contains(SDLK_D) || controllable.keysHeld.contains(SDLK_RIGHT))
                        rb.linearVelocity.x = 50.0f;
                }
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