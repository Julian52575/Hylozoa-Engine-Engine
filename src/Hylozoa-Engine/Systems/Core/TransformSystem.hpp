#ifndef TRANSFORM_SYSTEM_HPP
#define TRANSFORM_SYSTEM_HPP

#include <iostream>
#include "Hylozoa-Engine/Systems/Systems.hpp"
#include "Hylozoa-Engine/Components/Core/Transform2D.hpp"
#include "Hylozoa-Engine/Components/Core/Velocity2D.hpp"
#include <cstdint>

namespace Hylozoa {
    class TransformSystem : public System {
        public:
            const std::string& getName() const override { return this->_name; }

            void onStart() override {
                std::cout << "["<< this->_name << "] Start\n";
            }

            void onUpdate(float dt) override {
                if (this->_registry) {
                    auto view = this->_registry->view<Components::Transform2D, Components::Velocity2D>();
                    for (auto [entity, transform, velocity] : view.each()) {
                        std::cout << "[" << this->_name <<  "] Update frame (" << dt << "s) for entity: " << static_cast<uint32_t>(entity) << "\n";
                        std::cout << "  Position: (" << transform.position.first << ", " << transform.position.second << ")\n";
                        transform.position.first += velocity.velocity.first * dt;
                        transform.position.second += velocity.velocity.second * dt;
                        std::cout << "  New Position: (" << transform.position.first << ", " << transform.position.second << ")\n";
                    }
                }
            }

            void onEnd() override {
                std::cout << "["<< this->_name << "] End\n";
            }
        private:
            std::string _name = "TransformSystem";
    };
}
#endif // TRANSFORM_SYSTEM_HPP
