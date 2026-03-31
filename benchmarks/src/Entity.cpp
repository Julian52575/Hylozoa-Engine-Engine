#include <benchmark/benchmark.h>
#include <iostream>
#include <format>
#include <Hylozoa-Engine/Core/Engine.hpp>

#include "Hacks.hpp"

static void BM_CreateEntity(benchmark::State& state) {
    auto name = "BM_CreateEntity";
    auto jsonStr = std::format(R"({{
        "name": "{0}",
        "verbose": false,
        "debugLevel": 0
    }})", name);
    std::istringstream jsonStream(jsonStr);
    Hylozoa::Engine engine(Hylozoa::EngineMode::HEADLESS, jsonStream);

    for (auto _ : state) {
      auto entity = engine.scene().spawnEntity("BM_Entity");

      benchmark::DoNotOptimize(entity); // Prevent compiler optimizations
    }
}
// Register the function as a benchmark
BENCHMARK(BM_CreateEntity);