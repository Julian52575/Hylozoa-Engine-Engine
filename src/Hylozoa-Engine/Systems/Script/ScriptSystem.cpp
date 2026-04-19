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
}

void ScriptSystem::onUpdate(float dt) {
    auto view = _registry.view<Components::Script, Components::HylozoaInternal::SceneActiveTag>();

    for (auto entity : view) {
        auto &scriptComponent = view.get<Components::Script>(entity);
        Entity wrap = Entity::fromHandle(entity, _registry);
        if (scriptComponent.onUpdate.valid()) {
            scriptComponent.onUpdate(wrap, dt);
        }
    }
}

void ScriptSystem::onSceneLoaded(const uint64_t sceneId) {
    // Nothing to do for now
}

void ScriptSystem::onSceneUnloaded(const uint64_t sceneId) {
    // Nothing to do for now
}

void ScriptSystem::onEnd() {
    // Nothing to do for now
}


}