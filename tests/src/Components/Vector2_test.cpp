#include <gtest/gtest.h>
#include "Hylozoa-Engine/Components/Vector2.hpp"

// Demonstrate some basic assertions.
TEST(Vector2f, vector2f) {
    float x = 10.1f;
    float y = 20.2f;
    Hylozoa::Components::Vector2f v{x, y};

    EXPECT_EQ(v.x, x);
    EXPECT_EQ(v.y, y);
}

// Demonstrate some basic assertions.
TEST(Vector2i, vector2i) {
    int x = 10;
    int y = 20;
    Hylozoa::Components::Vector2i v{x, y};

    EXPECT_EQ(v.x, x);
    EXPECT_EQ(v.y, y);
}