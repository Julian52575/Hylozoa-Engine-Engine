#include <SDL3/SDL.h>

#include "Hylozoa-Engine/Systems/Input/Input.hpp"
#include "Hylozoa-Engine/Components/Input/Controllable.hpp"

#include <iostream>

namespace Hylozoa::Systems {

    Input::Input() {}

    void Input::onStart() {
        std::cout << "[" << this->_name << "] Start\n";
    }

    void Input::onUpdate(float) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT:
                    std::cout << "[Input] Quit event detected\n";
                    // Engine::getInstance().stop();  // arrête ton moteur proprement
                    break;

                case SDL_EVENT_KEY_DOWN:
                    for (auto &entity :this->_registry->view<Hylozoa::Components::Controllable>()) {
                        auto &inp = this->_registry->get<Hylozoa::Components::Controllable>(entity);
                        inp.keysHeld.insert(event.key.key);
                    }
                    break;
                case SDL_EVENT_KEY_UP:
                    for (auto &entity :this->_registry->view<Hylozoa::Components::Controllable>()) {
                        auto &inp = this->_registry->get<Hylozoa::Components::Controllable>(entity);
                        inp.keysHeld.erase(event.key.key);
                    }
                    break;

                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    std::cout << "Mouse button pressed at (" 
                            << event.button.x << ", " << event.button.y << ")\n";
                    break;

                default:
                    break;
            }
        }
    }

    void Input::onEnd() {
        std::cout << "[" << this->_name << "] End\n";
    }

    const std::string &Input::getName() const {
        return this->_name;
    }

} // namespace Hylozoa::Systems
