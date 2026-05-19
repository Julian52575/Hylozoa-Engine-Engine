/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** EngineState Component [header]
*/

#ifndef ENGINESTATE_HPP_
#define ENGINESTATE_HPP_

namespace Hylozoa::Components::HylozoaInternal {
/**
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

/**
 * @struct EngineMode
 * @brief Component to store the Running mode of the engine
 *
 * This component holds the current Running mode of the engine (Headless or Normal)
 *
 * @var enum Mode
 *      The current Mode of the engine
 */
struct EngineMode {
    enum class Mode { NORMAL, HEADLESS };

    Mode currentMode{Mode::NORMAL}; // current Engine mode
};
  
/**
 * @struct PendingDestruction
 * @brief Tag Component to mark entities that are pending destruction.
 * 
 * This component is used to mark entities that should be destroyed at the begining of the next update cycle.
 */
struct PendingDestruction {};

} // namespace Hylozoa::Components::HylozoaInternal

#endif /* !ENGINESTATE_HPP_ */