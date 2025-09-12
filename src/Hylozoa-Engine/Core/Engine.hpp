/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Heart Class of the Hylozoa Engine [header]
*/

#ifndef ENGINE_HPP_
#define ENGINE_HPP_

#include <flecs.h>
#include <iostream>

namespace Hylozoa {

enum class EngineState {
    RUNNING,
    PAUSED,
    STOPPED
};

class Engine {
    public:
        Engine();
        ~Engine() = default;

        flecs::world &getWorld() { return m_world; }
        void stop() { m_isRunning = false; }
        void runTick(int tick = 1);
        void run();

    private:
        flecs::world m_world;
        bool m_isRunning = false;
        EngineState m_state = EngineState::STOPPED;
};

} // namespace Hylozoa

#endif /* !ENGINE_HPP_ */