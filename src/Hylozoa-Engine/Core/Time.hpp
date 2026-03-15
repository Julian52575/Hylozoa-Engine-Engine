/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Time Core [header file]
*/

#pragma once

#include <entt/entt.hpp>

namespace Hylozoa {
/**
 * @class Time
 * @brief Manages time-related functionalities for the engine.
 */
class Time {
  public:
    Time(entt::registry &registry) : m_registry(registry) {};
    ~Time() = default;

    /**
     * @brief Returns the current game time in seconds (scaled by time scale).
     * 
     * @return double 
     */
    double gameTime() const;
    /**
     * @brief Returns the total time since the engine started in seconds (unscaled).
     * 
     * @return double 
     */
    double totalTime() const;
    /**
     * @brief Returns the delta time (time since last frame) in seconds, scaled by time scale.
     * 
     * @return float 
     */
    float deltaTime() const;
    /**
     * @brief Returns the real delta time (time since last frame) in seconds, unscaled by time scale.
     * 
     * @return float 
     */
    float realDeltaTime() const;
    /**
     * @brief Set the time scale for the game time. A time scale of 1.0 means normal speed, 0.5 means half speed, and 2.0 means double speed, etc...
     * 
     * @param scale 
     */
    void setTimeScale(float scale);
    /**
     * @brief the main method to update the time values.
     * 
     * This should be called once per frame, typically at the beginning of the main game loop. It calculates the real delta time, applies the time scale to get the scaled delta time, and updates the total game time and total time accordingly.
     * 
     * @warning Should not be called manually by users, as it is automatically handled by the engine's main loop.
     */
    void updateTime();

  private:
    entt::registry &m_registry;
};

} // namespace Hylozoa
