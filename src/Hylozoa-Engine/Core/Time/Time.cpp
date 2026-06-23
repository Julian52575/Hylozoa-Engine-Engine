/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Time Core [source file]
*/

#include "Hylozoa-Engine/Core/Time/Time.hpp"
#include "Hylozoa-Engine/Components/Context/EngineContext.hpp"
#include "Hylozoa-Engine/Components/Context/Time.hpp"

namespace Hylozoa {
double Time::gameTime() const {
    auto &time = m_registry.ctx().get<Components::HylozoaInternal::Time>();

    return time.totalGameTime;
}

double Time::totalTime() const {
    auto &time = m_registry.ctx().get<Components::HylozoaInternal::Time>();

    return time.totalTime;
}

float Time::deltaTime() const {
    auto &time = m_registry.ctx().get<Components::HylozoaInternal::Time>();

    return time.deltaTime;
}

float Time::realDeltaTime() const {
    auto &time = m_registry.ctx().get<Components::HylozoaInternal::Time>();

    return time.realDelta;
}

void Time::setTimeScale(float scale) {
    auto &time = m_registry.ctx().get<Components::HylozoaInternal::Time>();

    time.timeScale = scale;
}

void Time::reset() {
    auto &time = m_registry.ctx().get<Components::HylozoaInternal::Time>();

    time.totalGameTime = 0.f;
    time.totalTime = 0.f;
    time.deltaTime = 0.f;
    time.realDelta = 0.f;
    time.accumulator = 0.f;
    time.frameFixedSteps = 0;
}

void Time::updateTime(float elapsedTime) {
    auto &time =
        m_registry.ctx().get<Hylozoa::Components::HylozoaInternal::Time>();
    auto &state = m_registry.ctx()
                      .get<Hylozoa::Components::HylozoaInternal::EngineState>();

    if (time.realDelta > time.MAX_DELTA_TIME)
        time.realDelta = time.MAX_DELTA_TIME;

    if (state.currentState ==
        Hylozoa::Components::HylozoaInternal::EngineState::State::PAUSED) {
        time.deltaTime = 0.f;
    } else {
        time.deltaTime = time.realDelta * time.timeScale;
    }

    time.accumulator += time.deltaTime;

    time.totalTime += time.realDelta;
    time.totalGameTime += time.deltaTime;
    time.frameFixedSteps = 0;
}



} // namespace Hylozoa