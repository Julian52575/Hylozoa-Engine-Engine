#include <gtest/gtest.h>
#include "Hylozoa-Engine/Components/Vector2.hpp"

// Demonstrate some basic assertions.
TEST(Vector2, vector2) {
    Hylozoa::Components::Vector2 v{10, 20};

    EXPECT_EQ(v.x, 10);
    EXPECT_EQ(v.y, 20);
}
