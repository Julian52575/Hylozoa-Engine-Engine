/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Heart Class of the Hylozoa Engine [source file]
*/
#include "Engine.hpp"
#include "Hylozoa-Engine/Systems/Movement/Movement.hpp"
#include "Hylozoa-Engine/Systems/Physics/Collision.hpp"
#include "Hylozoa-Engine/Systems/Renderer/Renderer.hpp"
#include "Hylozoa-Engine/Systems/Transform/Transform.hpp"

#include "Hylozoa-Engine/Components/Context/EngineContext.hpp"
#include "Hylozoa-Engine/Components/Context/Events.hpp"
#include "Hylozoa-Engine/Components/Context/Input.hpp"
#include "Hylozoa-Engine/Components/Context/Time.hpp"
#include "Hylozoa-Engine/Components/Context/SceneState.hpp"
#include <chrono>

namespace Hylozoa {

Engine::Engine(EngineMode mode) {
    std::cout << "[Engine] Initializing Hylozoa Engine..." << std::endl;

  // Initialize Engine Context Components
  m_registry.ctx().emplace<Components::HylozoaInternal::EngineState>();
  m_registry.ctx().emplace<Components::HylozoaInternal::EngineEvents>();
  m_registry.ctx().emplace<Components::HylozoaInternal::Time>();
  m_registry.ctx().emplace<Components::HylozoaInternal::InputState>();
  m_registry.ctx().emplace<Components::HylozoaInternal::MouseState>();
  m_registry.ctx().emplace<Components::HylozoaInternal::SceneState>();
  m_registry.ctx().emplace<Components::HylozoaInternal::EventsDispatcher>();
  //------------------------------

  m_sceneManager.initialize();
  m_systemManager.initialize();

  m_systemManager.registerSystem<ParentChildSystem>(0);
  m_systemManager.registerSystem<UpdateTransformSystem>(1);
  m_systemManager.registerSystem<Systems::Movement>(3);
  if (mode == EngineMode::Normal)
    m_systemManager.registerSystem<Systems::Renderer>(99);

    m_systemManager.registerFixedSystem<CollisionSystem>(0);

    m_systemManager.orderAllSystems();
    m_systemManager.startAll();

    std::cout << "[Engine] Hylozoa Engine initialized." << std::endl;
}

void Engine::run() {
    using clock = std::chrono::high_resolution_clock;

  auto &state =
      m_registry.ctx().get<Hylozoa::Components::HylozoaInternal::EngineState>();
  auto &time =
      m_registry.ctx().get<Hylozoa::Components::HylozoaInternal::Time>();
  auto &events =
      m_registry.ctx().get<Hylozoa::Components::HylozoaInternal::EngineEvents>();

    state.currentState =
        Hylozoa::Components::HylozoaInternal::EngineState::State::RUNNING;

    auto previous = clock::now();

    while (state.currentState !=
           Hylozoa::Components::HylozoaInternal::EngineState::State::
               STOPPED) { // pitié faut que je trouve un truc pour racourcir les
                          // call aux states
        auto current = clock::now();
        std::chrono::duration<float> elapsed = current - previous;
        previous = current;

        time.realDelta = elapsed.count();

        time.accumulator += time.deltaTime;

        if (state.currentState ==
            Hylozoa::Components::HylozoaInternal::EngineState::State::PAUSED) {
            time.deltaTime = 0.f;
        } else {
            time.deltaTime = time.realDelta * time.timeScale;
        }

        m_inputManager.pollEvents();

        if (events.shouldQuit) {
            state.currentState = Hylozoa::Components::HylozoaInternal::
                EngineState::State::STOPPED;
            break;
        }

        while (time.accumulator >= time.fixedDelta) {
            fixedUpdate(time.fixedDelta); // physics Update
            time.accumulator -= time.fixedDelta;
            time.frameFixedSteps++;
        }

        onUpdate(time.deltaTime); // normal Update
    }
}

void Engine::runTick(int tick) {
    auto &time =
        m_registry.ctx().get<Hylozoa::Components::HylozoaInternal::Time>();
    for (int i = 0; i < tick; ++i) {
        m_systemManager.updateAll(time.fixedDelta);
    }
}

void Engine::runTick(float realDelta) {
    auto &time =
        m_registry.ctx().get<Hylozoa::Components::HylozoaInternal::Time>();
    auto &state = m_registry.ctx()
                      .get<Hylozoa::Components::HylozoaInternal::EngineState>();

    time.realDelta = realDelta;
    m_timeManager.updateTime();

    m_inputManager.beginFrame();

    while (time.accumulator >= time.fixedDelta) {
        fixedUpdate(time.fixedDelta); // physics Update
        time.accumulator -= time.fixedDelta;
        time.frameFixedSteps++;
    }

    onUpdate(time.deltaTime);
}

void Engine::stop() {
    auto &state = m_registry.ctx()
                      .get<Hylozoa::Components::HylozoaInternal::EngineState>();
    state.currentState =
        Hylozoa::Components::HylozoaInternal::EngineState::State::STOPPED;
}

void Engine::pause() {
    auto &state = m_registry.ctx()
                      .get<Hylozoa::Components::HylozoaInternal::EngineState>();
    state.currentState =
        Hylozoa::Components::HylozoaInternal::EngineState::State::PAUSED;
}

void Engine::fixedUpdate(float fixedDeltaTime) {
    m_systemManager.updateFixed(fixedDeltaTime);
}

void Engine::onUpdate(float deltaTime) { m_systemManager.update(deltaTime); }
} // namespace Hylozoa