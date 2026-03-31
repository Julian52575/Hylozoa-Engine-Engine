/*
** EPITECH PROJECT, 2026
** Hylozoa Engine
** File description:
** Audio related System [header]
*/

#ifndef AUDIO_SYSTEM_HPP_
#define AUDIO_SYSTEM_HPP_

#include "Hylozoa-Engine/Components/Context/Events.hpp"
#include "Hylozoa-Engine/Systems/Manager/Systems.hpp"

#include <entt/entt.hpp>

#include <iostream>

namespace Hylozoa {
namespace Systems {

class AudioSystem : public System {
  public:
    AudioSystem(entt::registry &registry) : System(registry) {}
    const std::string &getName() const override { return this->_name; }

    void onStart() override {
        std::cout << "[" << this->_name << "] Start\n";
        auto &dispatcher =
            this->_registry.ctx()
                .get<Components::HylozoaInternal::EventsDispatcher>();

        dispatcher.dispatcher.sink<Components::HylozoaInternal::OnNoiseEvent>()
            .connect<&AudioSystem::onNoiseEvent>(this);
    }

    void onUpdate(float dt) override { (void)dt; }

    void onEnd() override {
        auto &dispatcher =
            this->_registry.ctx()
                .get<Components::HylozoaInternal::EventsDispatcher>();
        dispatcher.dispatcher.sink<Components::HylozoaInternal::OnNoiseEvent>()
            .disconnect<&AudioSystem::onNoiseEvent>(this);
        std::cout << "[" << this->_name << "] End\n";
    }

    void onNoiseEvent(const Components::HylozoaInternal::OnNoiseEvent &event);

  private:
    std::string _name = "AudioSystem";
};

} // namespace Systems
} // namespace Hylozoa

#endif /* !AUDIO_SYSTEM_HPP_ */