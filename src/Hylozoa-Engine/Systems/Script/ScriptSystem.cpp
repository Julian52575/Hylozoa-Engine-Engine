/*
** EPITECH PROJECT, 2026
** Hylozoa Engine
** File description:
** Script System [Source]
*/

#include "ScriptSystem.hpp"

#include "Hylozoa-Engine/Systems/Manager/Systems.hpp"
#include "Hylozoa-Engine/Components/Components.hpp"
#include "Hylozoa-Engine/Core/Entities/Entity.hpp"
#include "Hylozoa-Engine/Core/Settings.hpp"

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

namespace Hylozoa::Systems {

void ScriptSystem::onStart() {
    if (Hylozoa::Settings::getInstance().getSettings().verbose) {
        std::cout << "[" << this->_name << "] Start\n";
    }
    auto &dispatcher =
    this->_registry.ctx()
        .get<Components::HylozoaInternal::EventsDispatcher>();

    dispatcher.dispatcher.sink<Components::HylozoaInternal::OnNoiseEvent>()
        .connect<&ScriptSystem::onNoiseEvent>(this);

}

void ScriptSystem::onUpdate(float dt) {
    auto view = _registry.view<Components::Script, Components::HylozoaInternal::SceneActiveTag>();

    for (auto entity : view) {
        auto &scriptComponent = view.get<Components::Script>(entity);
        Entity wrap = Entity::fromHandle(entity, _registry);
        auto name = wrap.getName();
        if (!scriptComponent.onUpdate.valid()) {
            continue;
        }
        if (scriptComponent.scriptFile.empty()) {
            continue;
        }
        scriptComponent.onUpdate(wrap, dt);
    }
}

void ScriptSystem::onSceneLoaded(const uint64_t sceneId) {
    // Nothing to do for now
}

void ScriptSystem::onSceneUnloaded(const uint64_t sceneId) {
    // Nothing to do for now
}

void ScriptSystem::onNoiseEvent(const Components::HylozoaInternal::OnNoiseEvent &event) {
    auto entity = Entity::fromHandle(event.source, _registry);

    auto listenerView =
        _registry.view<Components::Script, Components::NoiseListener, Components::WorldTransform, Components::HylozoaInternal::SceneActiveTag>();

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
            if (Hylozoa::Settings::getInstance().getSettings().verbose) {
                std::cout << "Listener " << listenerName
                          << " is out of range for noise '" << event.noiseName
                          << "' emitted by " << entity.getName() << "\n";
            }
            continue;
        }

        auto& scriptComponent = listenerView.get<Components::Script>(listenerEntity);
        if (scriptComponent.onNoise.valid()) {
            Hylozoa::Components::HylozoaInternal::NoiseInfo noiseInfoStruct{
                event.noiseName,
                event.position,
                direction
            };

            scriptComponent.onNoise(entity, noiseInfoStruct);
            std::cout << "FIre on noise function\n";
        }


        if (Hylozoa::Settings::getInstance().getSettings().verbose) {
    
            // std::string noiseInfo =
            //     "Noise '" + event.noiseName + "' emitted by " +
            //     entity.getName() + " at position (" +
            //     std::to_string(event.position.x) + ", " +
            //     std::to_string(event.position.y) + ") comming from (" +
            //     std::to_string(direction.x) + ", " +
            //     std::to_string(direction.y) + ") received by listener " +
            //     listenerName;

            // std::cout << noiseInfo << std::endl;
        }

        
    }
}

void ScriptSystem::onEnd() {
    // Nothing to do for now
}


}