/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Heart Class of the Hylozoa Engine [source file]
*/

#include "Engine.hpp"
#include "Hylozoa-Engine/Systems/Movements.hpp"

namespace Hylozoa {

Engine::Engine()
{
    std::cout << "[Engine] Initializing Hylozoa Engine..." << std::endl;

    // Register systems here
    registerMovementsSystem(this->m_world);


    std::cout << "[Engine] Hylozoa Engine initialized." << std::endl;
}

void Engine::run()
{
    this->m_isRunning = true;

    while (this->m_isRunning) {
        this->m_world.progress();
    }
}

void Engine::runTick(int tick)
{
    for (int i = 0; i < tick; ++i) {
        this->m_world.progress();
    }
}

} // namespace Hylozoa