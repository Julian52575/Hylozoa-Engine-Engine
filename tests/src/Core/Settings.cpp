#include <gtest/gtest.h>
#include <Hylozoa-Engine/Core/Settings.hpp>
#include <format>

TEST(Settings, TwoInstanceSameSettingsPtr) {
    Hylozoa::Settings settings1;
    Hylozoa::Settings settings2;

    EXPECT_EQ(&settings1.get(), &settings2.get());
}

TEST(Settings, LoadFromJson) {
    auto name = "LoadFromJson";
    auto jsonStr = std::format(R"({{
        "name": "{0}",
        "verbose": true,
        "debugLevel": 42
    }})", name);
    std::istringstream jsonStream(jsonStr);
    Hylozoa::Settings settings(jsonStream);

    EXPECT_TRUE(settings.get().verbose);
    EXPECT_EQ(settings.get().debugLevel, 42);
    EXPECT_EQ(settings.get().name, name);
}