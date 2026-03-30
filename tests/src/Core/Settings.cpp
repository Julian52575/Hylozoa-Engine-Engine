#include <gtest/gtest.h>
#include <Hylozoa-Engine/Core/Settings.hpp>
#include <format>

TEST(Settings, TwoInstanceSameSettingsPtr) {
    EXPECT_EQ(&Hylozoa::Settings::getInstance().getSettings(), &Hylozoa::Settings::getInstance().getSettings());
}

TEST(Settings, LoadFromJson) {
    auto name = "LoadFromJson";
    auto jsonStr = std::format(R"({{
        "name": "{0}",
        "verbose": true,
        "debugLevel": 42
    }})", name);
    std::istringstream jsonStream(jsonStr);
    Hylozoa::Settings::getInstance().load(jsonStream);

    EXPECT_TRUE(Hylozoa::Settings::getInstance().getSettings().verbose);
    EXPECT_EQ(Hylozoa::Settings::getInstance().getSettings().debugLevel, 42);
    EXPECT_EQ(Hylozoa::Settings::getInstance().getSettings().name, name);
}