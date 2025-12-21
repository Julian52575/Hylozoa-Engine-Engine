/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** EngineState Component [header]
*/

#ifndef ENGINESTATE_HPP_
#define ENGINESTATE_HPP_

namespace Hylozoa::Components::HylozoaInternal {
/*
 * @struct EngineState
 * @brief Component to store the state of the engine.
 * This component holds the current state of the engine, such as running,
 * paused, or stopped.
 *
 * @var enum State
 *      The current state of the engine.
 */
struct EngineState {
  enum class State { RUNNING, PAUSED, STOPPED };

  State currentState{State::STOPPED}; // current Engine state
};

} // namespace Hylozoa::Components::HylozoaInternal

#endif /* !ENGINESTATE_HPP_ */