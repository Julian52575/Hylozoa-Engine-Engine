#ifndef RENDER_SYSTEM_HPP
#define RENDER_SYSTEM_HPP

#include "Hylozoa-Engine/Components/Transform/Transform.hpp"
#include "Hylozoa-Engine/Systems/Manager/Systems.hpp"
#include "Hylozoa-Engine/Core/Settings.hpp"
#include <iostream>

namespace Hylozoa {
class RenderSystem : public System {
  public:
    RenderSystem(entt::registry &registry) : System(registry) {}
    const std::string &getName() const override { return this->_name; }

    void onStart() override {
      if (Hylozoa::Settings::getInstance().getSettings().verbose) {
        std::cout << "[" << this->_name << "] Start\n"; }
    }

    void onUpdate(float dt) override {
        auto view = this->_registry.view<Components::LocalTransform>();
        for (auto entity : view) {
            if (Hylozoa::Settings::getInstance().getSettings().verbose) {
                std::cout << "[" << this->_name << "] Update frame (" << dt
                      << "s) for entity: " << static_cast<uint32_t>(entity)
                      << "\n";
            }
        }
    }

    void onEnd() override {
        if (Hylozoa::Settings::getInstance().getSettings().verbose) {
            std::cout << "[" << this->_name << "] End\n"; 
        }
    }

  private:
    std::string _name = "RenderSystem";
};
} // namespace Hylozoa
#endif // RENDER_SYSTEM_HPP
