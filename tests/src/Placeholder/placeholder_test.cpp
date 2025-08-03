#include <gtest/gtest.h>
#include "Placeholder.hpp"

// Demonstrate some basic assertions.
TEST(PlaceholderTest, BasicAssertions) {
    Hylozoa::Placeholder pl;

    EXPECT_EQ(pl.returnInt(42), 42);
}