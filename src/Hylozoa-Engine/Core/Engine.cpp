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
    registerLocalToWorldSystem(this->m_world);


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

Entity Engine::createEntity(const std::string& name)
{
    return Entity(this->m_world.entity(name.c_str()));
}

Entity Engine::createSpacialEntity(const std::string& name)
{
    auto e = this->m_world.entity(name.c_str());
    e.set<LocalTransform>({{0, 0}, {1, 1}, 0});
    e.set<GlobalTransform>({{0, 0}, {1, 1}, 0});
    return Entity(e);
}
} // namespace Hylozoa