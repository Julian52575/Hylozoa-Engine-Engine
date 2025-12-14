#include <gtest/gtest.h>
#include "InputFixture.hpp"


TEST_F(InputFixture, KeyPressedLastsOneFrame) {
    engine.beginFrame();
    inputState.keysPressed[SDL_SCANCODE_E] = true;
    inputState.keysHeld[SDL_SCANCODE_E] = true;

    EXPECT_TRUE(engine.input().isKeyDown("E"));
    EXPECT_TRUE(engine.input().isKeyHeld("E"));

    engine.runTick(0.016f); // next frame

    EXPECT_FALSE(engine.input().isKeyDown("E"));
    EXPECT_TRUE(engine.input().isKeyHeld("E"));
}

TEST_F(InputFixture, KeyReleaseIsDetected) {
    engine.beginFrame();

    inputState.keysHeld[SDL_SCANCODE_E] = true;

    engine.runTick(0.016f);

    engine.beginFrame();
    inputState.keysReleased[SDL_SCANCODE_E] = true;
    inputState.keysHeld[SDL_SCANCODE_E] = false;

    EXPECT_TRUE(engine.input().isKeyUp("E"));
    EXPECT_FALSE(engine.input().isKeyHeld("E"));
}