/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Time Core [source file]
*/

#include "Hylozoa-Engine/Core/Time.hpp"
#include "Hylozoa-Engine/Components/Context/Time.hpp"

namespace Hylozoa
{
    double Time::gameTime() const
    {
        auto &time = m_registry.ctx().get<Components::HylozoaInternal::Time>();

        return time.totalGameTime;
    }

    double Time::totalTime() const
    {
        auto &time = m_registry.ctx().get<Components::HylozoaInternal::Time>();

        return time.totalTime;
    }

    float Time::deltaTime() const
    {
        auto &time = m_registry.ctx().get<Components::HylozoaInternal::Time>();

        return time.deltaTime;
    }

    float Time::realDeltaTime() const
    {
        auto &time = m_registry.ctx().get<Components::HylozoaInternal::Time>();

        return time.realDelta;
    }

    void Time::setTimeScale(float scale)
    {
        auto &time = m_registry.ctx().get<Components::HylozoaInternal::Time>();

        time.timeScale = scale;
    }
    
} // namespace Hylozoa