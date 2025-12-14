#include "EngineFixture.hpp"
#include "Hylozoa-Engine/Components/Context/Time.hpp"

TEST_F(HeadlessEngineFixture, TimeDeltaComputedCorrectly) {
    engine.runTick(0.016f);

    auto& time = engine.getRegistry().ctx().get<Hylozoa::Components::HylozoaInternal::Time>();

    EXPECT_FLOAT_EQ(time.realDelta, 0.016f);
    EXPECT_FLOAT_EQ(time.deltaTime, 0.016f);
}

TEST_F(HeadlessEngineFixture, PauseStopsGameTime) {
    auto& state = engine.getRegistry().ctx().get<Hylozoa::Components::HylozoaInternal::EngineState>();
    auto& time  = engine.getRegistry().ctx().get<Hylozoa::Components::HylozoaInternal::Time>();

    state.currentState = Hylozoa::Components::HylozoaInternal::EngineState::State::PAUSED;

    engine.runTick(0.016f);

    EXPECT_FLOAT_EQ(time.deltaTime, 0.0f);
    EXPECT_FLOAT_EQ(time.realDelta, 0.016f);
}

TEST_F(HeadlessEngineFixture, FixedUpdateRunsCorrectSteps) {
    auto& time = engine.getRegistry().ctx().get<Hylozoa::Components::HylozoaInternal::Time>();
    time.fixedDelta = 0.02f;

    engine.runTick(0.05f);

    EXPECT_EQ(time.frameFixedSteps, 2);
}