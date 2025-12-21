/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Time Context Component [header]
*/

#ifndef TIME_HPP_
#define TIME_HPP_

namespace Hylozoa::Components::HylozoaInternal {

/*
 * @struct Time
 * @brief Component to store time-related data.
 *
 * This component holds various time-related variables used for
 * managing the engine's timing and update cycles.
 */
struct Time {
  float deltaTime{0.f};               // variable dt
  float realDelta{0.f};               // real dt (unscaled)
  float fixedDelta{1.f / 60.f};       // fixed dt for physics
  float timeScale{1.f};               // pause & slow-mo
  double accumulator{0.0};            // for fixed timestep (mainly physics)
  double totalTime{0.0};              // total elapsed time since engine start
  double totalGameTime{0.0};          // total game time (stopped when paused)
  int frameFixedSteps{0};             // number of fixed steps this frame
  const float MAX_DELTA_TIME = 0.25f; // max delta time to avoid spiral of death
};

} // namespace Hylozoa::Components::HylozoaInternal

#endif /* !TIME_HPP_ */