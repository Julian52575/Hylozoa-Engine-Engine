/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Time Core [header file]
*/

#pragma once

#include <entt/entt.hpp>

namespace Hylozoa
{
    class Time
    {
    public:
        Time(entt::registry registry) : m_registry(registry) {};
        ~Time() = default;

        // Returns the current time in seconds since the engine started
        double gameTime() const;
        // Returns the total time in seconds since the engine started
        double totalTime() const;
        // Returns the delta time (scaled)
        float deltaTime() const;
        // Returns the real delta time (unscaled)
        float realDeltaTime() const;
        // Sets the time scale (for slow-motion or pausing)
        void setTimeScale(float scale);
    private:
        entt::registry& m_registry;
    };
    
} // namespace Hylozoa
