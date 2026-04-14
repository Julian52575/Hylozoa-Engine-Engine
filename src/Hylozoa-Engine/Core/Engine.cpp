/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Heart Class of the Hylozoa Engine [source file]
*/
#include "Engine.hpp"
#include "Layers/LayerManager.hpp"

#include "Hylozoa-Engine/Systems/Audio/AudioSystem.hpp"
#include "Hylozoa-Engine/Systems/Movement/Movement.hpp"
#include "Hylozoa-Engine/Systems/Physics/Physics.hpp"
#include "Hylozoa-Engine/Systems/Renderer/Renderer.hpp"
#include "Hylozoa-Engine/Systems/Transform/Transform.hpp"

#include "Hylozoa-Engine/Components/Context/EngineContext.hpp"
#include "Hylozoa-Engine/Components/Context/Events.hpp"
#include "Hylozoa-Engine/Components/Context/Input.hpp"
#include "Hylozoa-Engine/Components/Context/SceneState.hpp"
#include "Hylozoa-Engine/Components/Context/Time.hpp"

#include "Hylozoa-Engine/Core/Settings.hpp"
#include <chrono>

namespace Hylozoa {

Engine::Engine(EngineMode mode) : mode(mode) { }

Engine::Engine(const std::string &settingsPath) : mode(EngineMode::NORMAL) {
    loadSettings(settingsPath);
}

Engine::Engine(EngineMode mode, const std::string &settingsPath) : mode(mode) {
    loadSettings(settingsPath);
}

Engine::Engine(EngineMode mode, std::istream &jsonStream) : mode(mode) {
    loadSettings(jsonStream);
}

void Engine::run() {
    using clock = std::chrono::high_resolution_clock;

    auto &state = m_registry.ctx()
                      .get<Hylozoa::Components::HylozoaInternal::EngineState>();
    auto &time =
        m_registry.ctx().get<Hylozoa::Components::HylozoaInternal::Time>();
    auto &events =
        m_registry.ctx()
            .get<Hylozoa::Components::HylozoaInternal::EngineEvents>();

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

        m_timeManager.updateTime(elapsed.count());
        time.realDelta = elapsed.count();

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
    m_timeManager.updateTime(realDelta);

    m_inputManager.beginFrame();

    fixedUpdate(time.fixedDelta);
    time.accumulator -= time.fixedDelta;

    onUpdate(time.deltaTime);
}

void Engine::stop() {
    auto &state = m_registry.ctx()
                      .get<Hylozoa::Components::HylozoaInternal::EngineState>();
    state.currentState =
        Hylozoa::Components::HylozoaInternal::EngineState::State::STOPPED;
    time().reset();
    m_sceneManager.clearScenes();
}

void Engine::pause() {
    auto &state = m_registry.ctx()
                      .get<Hylozoa::Components::HylozoaInternal::EngineState>();
    state.currentState =
        Hylozoa::Components::HylozoaInternal::EngineState::State::PAUSED;
}

void Engine::unpause() {
    auto &state = m_registry.ctx()
                      .get<Hylozoa::Components::HylozoaInternal::EngineState>();
    state.currentState =
        Hylozoa::Components::HylozoaInternal::EngineState::State::RUNNING;
}

void Engine::shutdown() {
    auto &state = m_registry.ctx()
        .get<Hylozoa::Components::HylozoaInternal::EngineState>();
    state.currentState =
        Hylozoa::Components::HylozoaInternal::EngineState::State::STOPPED;

    m_sceneManager.clearScenes();
    m_systemManager.endAll();
}

void Engine::init() {
    if (Hylozoa::Settings::getInstance().getSettings().verbose) {
        std::cout << "[Engine] Initializing Hylozoa Engine..." << std::endl;
    }

    initializeContextComponents();

    initializeManagers();

    initializeSystems();

    if (Hylozoa::Settings::getInstance().getSettings().verbose) {
        std::cout << "[Engine] Hylozoa Engine initialized." << std::endl;
    }
}

void Engine::fixedUpdate(float fixedDeltaTime) {
    m_systemManager.updateFixed(fixedDeltaTime);
}

void Engine::onUpdate(float deltaTime) { m_systemManager.update(deltaTime); }

void Engine::loadSettings(std::istream &jsonStream) {
    Hylozoa::Settings::getInstance().load(jsonStream);

    if (Hylozoa::Settings::getInstance().getSettings().verbose) {
        std::cout << "Verbose mode active, printing settings loaded from "
                     "'<jsonStream>':"
                  << std::endl
                  << Hylozoa::Settings::getInstance().getSettings()
                  << std::endl;
    }
}
void Engine::loadSettings(const std::string &settingsPath) {
    auto stream = std::ifstream(settingsPath);

    this->loadSettings(stream);
}

void Engine::initializeContextComponents()
{
    m_registry.ctx().emplace<Components::HylozoaInternal::EngineState>();
    m_registry.ctx().emplace<Components::HylozoaInternal::EngineEvents>();
    m_registry.ctx().emplace<Components::HylozoaInternal::Time>();
    m_registry.ctx().emplace<Components::HylozoaInternal::InputState>();
    m_registry.ctx().emplace<Components::HylozoaInternal::MouseState>();
    m_registry.ctx().emplace<Components::HylozoaInternal::SceneState>();
    m_registry.ctx().emplace<Components::HylozoaInternal::EventsDispatcher>();

    m_registry.ctx().emplace<TextureManager>();
    m_registry.ctx().emplace<SoundManager>();
}
void Engine::initializeManagers()
{
    m_sceneManager.initialize();
    m_systemManager.initialize();
    LayerManager::instance();
}
void Engine::initializeSystems()
{
    m_systemManager.registerSystem<Systems::ParentChildSystem>(0);
    m_systemManager.registerSystem<Systems::UpdateTransformSystem>(1);
    m_systemManager.registerSystem<Systems::Movement>(3);
    m_systemManager.registerSystem<Systems::AudioSystem>(4);
    if (mode == EngineMode::NORMAL)
        m_systemManager.registerSystem<Systems::Renderer>(99);

    m_systemManager.registerFixedSystem<Systems::PhysicsSystem>(0);

    m_systemManager.orderAllSystems();
    m_systemManager.startAll();
}

} // namespace Hylozoa