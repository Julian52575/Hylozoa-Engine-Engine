#include <benchmark/benchmark.h>
#include <iostream>
#include <format>
#include <Hylozoa-Engine/Core/Engine.hpp>

#include "Hacks.hpp"

static void BM_InitEngineNoConfig(benchmark::State& state)
{
    for (auto _ : state) {
        Hylozoa::Engine engine(Hylozoa::EngineMode::HEADLESS);

        benchmark::DoNotOptimize(engine);
    }
}

static void BM_InitEngineConfigPath(benchmark::State& state)
{
    for (auto _ : state) {
        Hylozoa::Engine engine(Hylozoa::EngineMode::HEADLESS, "benchmarks/src/settings.json");

        benchmark::DoNotOptimize(engine);
    }
}

static void BM_InitEngineConfigStreamDebuglvl0(benchmark::State& state)
{
    auto name = "BM_InitEngineConfigStreamDebuglvl0";
    auto jsonStr = std::format(R"({{
        "name": "{0}",
        "verbose": false,
        "debugLevel": 0
    }})", name);

    for (auto _ : state) {
        state.PauseTiming();
        std::istringstream jsonStream(jsonStr);
        state.ResumeTiming();
        Hylozoa::Engine engine(Hylozoa::EngineMode::HEADLESS, jsonStream);

        benchmark::DoNotOptimize(engine);
    }
}

static void BM_InitEngineConfigStreamDebugLevel1(benchmark::State& state)
{
    auto name = "BM_InitEngineConfigStreamDebuglvl1";
    auto jsonStr = std::format(R"({{
        "name": "{0}",
        "verbose": false,
        "debugLevel": 1
    }})", name);

    for (auto _ : state) {
        state.PauseTiming();
        std::istringstream jsonStream(jsonStr);
        state.ResumeTiming();
        Hylozoa::Engine engine(Hylozoa::EngineMode::HEADLESS, jsonStream);

        benchmark::DoNotOptimize(engine);
    }
}

static void BM_EngineDelete(benchmark::State& state)
{
    for (auto _ : state) {
        state.PauseTiming();
        alignas(Hylozoa::Engine) char buffer[sizeof(Hylozoa::Engine)];
        Hylozoa::Engine* engine = new (buffer) Hylozoa::Engine(Hylozoa::EngineMode::HEADLESS);
        state.ResumeTiming();

        benchmark::DoNotOptimize(engine);
        engine->~Engine();
    }
}


BENCHMARK(BM_InitEngineNoConfig);
BENCHMARK(BM_InitEngineConfigStreamDebuglvl0);
BENCHMARK(BM_InitEngineConfigStreamDebugLevel1);
BENCHMARK(BM_InitEngineConfigPath);
BENCHMARK(BM_EngineDelete);