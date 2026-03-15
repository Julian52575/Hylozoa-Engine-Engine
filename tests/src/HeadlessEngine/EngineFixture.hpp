#include <gtest/gtest.h>
#include "Hylozoa-Engine/Core/Engine.hpp"
#include "Hylozoa-Engine/Components/Context/EngineContext.hpp"

class HeadlessEngineFixture : public ::testing::Test
{
protected:
    Hylozoa::Engine engine;
private:
public:
    HeadlessEngineFixture() : engine(Hylozoa::EngineMode::HEADLESS) {
        auto& state = engine.getRegistry().ctx().get<Hylozoa::Components::HylozoaInternal::EngineState>();
        state.currentState = Hylozoa::Components::HylozoaInternal::EngineState::State::RUNNING;
    }

};

