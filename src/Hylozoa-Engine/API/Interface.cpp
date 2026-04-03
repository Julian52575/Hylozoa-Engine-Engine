/*
** EPITECH PROJECT, 2026
** Hylozoa Engine
** File description:
** API functions implementations
*/

#include "Interface.hpp"
#include "Hylozoa-Engine/Core/Engine.hpp"

static Hylozoa::Engine *globalEngine = nullptr;

void engine_create(char* settingsPath) {
    if (globalEngine == nullptr) {
        globalEngine = new Hylozoa::Engine(settingsPath);
    }
}

void engine_init() {
    if (globalEngine) {
        globalEngine->init();
    }
}

void engine_run() {
    if (globalEngine) {
        globalEngine->run();
    }
}

void engine_pause() {
    if (globalEngine) {
        globalEngine->pause();
    }
}

void engine_stop() {
    if (globalEngine) {
        globalEngine->stop();
    }
}

void engine_shutdown() {
    if (globalEngine) {
        delete globalEngine;
        globalEngine = nullptr;
    }
}