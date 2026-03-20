/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Audio System [Source]
*/

#include "AudioSystem.hpp"
#include "Hylozoa-Engine/Components/Components.hpp"
#include "Hylozoa-Engine/Core/Entity.hpp"

namespace Hylozoa {
namespace Systems {

void AudioSystem::onNoiseEvent(
    const Components::HylozoaInternal::OnNoiseEvent &event) {
    auto entity = Entity::fromHandle(event.source, _registry);

    auto listenerView =
        _registry.view<Components::NoiseListener, Components::WorldTransform>();

    for (auto listenerEntity : listenerView) {
        auto &listener =
            listenerView.get<Components::NoiseListener>(listenerEntity);
        auto &listenerTransform =
            listenerView.get<Components::WorldTransform>(listenerEntity);
        std::string listenerName =
            Entity::fromHandle(listenerEntity, _registry).getName();

        float distance =
            glm::distance(listenerTransform.position, event.position);

        glm::vec2 direction(1.0f, 0.0f);
        if (distance > 0.0001f)
            direction =
                glm::normalize(event.position - listenerTransform.position);

        if (distance > listener.hearingRange) {
            std::cout << "Listener " << listenerName
                      << " is out of range for noise '" << event.noiseName
                      << "' emitted by " << entity.getName() << "\n";
            continue;
        }

        std::string noiseInfo =
            "Noise '" + event.noiseName + "' emitted by " + entity.getName() +
            " at position (" + std::to_string(event.position.x) + ", " +
            std::to_string(event.position.y) + ") comming from (" +
            std::to_string(direction.x) + ", " + std::to_string(direction.y) +
            ") with intensity " + std::to_string(event.intensity) +
            " received by listener " + listenerName;
        std::cout << noiseInfo << std::endl;
    }
    std::cout << "test\n";
}

} // namespace Systems
} // namespace Hylozoa