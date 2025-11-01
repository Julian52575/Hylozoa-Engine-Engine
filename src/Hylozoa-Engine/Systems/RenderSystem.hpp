#ifndef RENDER_SYSTEM_HPP
#define RENDER_SYSTEM_HPP

#include <iostream>
#include "Hylozoa-Engine/Systems/Systems.hpp"
#include "Hylozoa-Engine/Components/Transform.hpp"

namespace Hylozoa {
    class RenderSystem : public System {
        public:
            const std::string& getName() const override { return this->_name; }

            void onStart() override {
                std::cout << "["<< this->_name << "] Start\n";
            }

            void onUpdate(float dt) override {
                if (this->_registry) {
                    auto view = this->_registry->view<LocalTransform>();
                    for (auto entity : view) {
                        std::cout << "[" << this->_name <<  "] Update frame (" << dt << "s) for entity: " << static_cast<uint32_t>(entity) << "\n";
                    }
                }
            }

            void onEnd() override {
                std::cout << "["<< this->_name << "] End\n";
            }
        private:
            std::string _name = "RenderSystem";
    };
}
#endif // RENDER_SYSTEM_HPP
