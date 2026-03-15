#include <benchmark/benchmark.h>
#include <iostream>
#include <Hylozoa-Engine/Core/Engine.hpp>
#include "Hacks.hpp"

static void BM_CreateEntity(benchmark::State& state) {
    Hylozoa::Engine engine(Hylozoa::EngineMode::HEADLESS);
    // Perform setup here
    for (auto _ : state) {
      auto entity = engine.scene().spawnEntity("BM_Entity");

      //escape(&entity); // Prevent compiler optimizations
    }
}
// Register the function as a benchmark
BENCHMARK(BM_CreateEntity);