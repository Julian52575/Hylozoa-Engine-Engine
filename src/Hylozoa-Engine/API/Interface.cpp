/*
** EPITECH PROJECT, 2026
** Hylozoa Engine
** File description:
** API functions implementations
*/

#include "nlohmann/json.hpp"

#include "Interface.hpp"
#include "Hylozoa-Engine/Core/Engine.hpp"
#include "Hylozoa-Engine/Components/Scene/UUID.hpp"

static Hylozoa::Engine *globalEngine = nullptr;

// --------------------ENGINE API FUNCTIONS IMPLEMENTATIONS-------------------

void engine_create(const char* settingsPath) {
    if (globalEngine != nullptr) {
        return;
    }
    if (settingsPath == nullptr) {
        globalEngine = new Hylozoa::Engine(Hylozoa::EngineMode::NORMAL);
    } else { 
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

void engine_unpause() {
    if (globalEngine) {
        globalEngine->unpause();
    }
}

void engine_stop() {
    if (globalEngine) {
        globalEngine->stop();
    }
}

void engine_shutdown() {
    if (not globalEngine) {
        return;
    }
    delete globalEngine;
    globalEngine = nullptr;
}

// --------------------SCENE API FUNCTIONS IMPLEMENTATIONS--------------------

bool scene_create(const char* jsonContent) {
    if (jsonContent == nullptr) {
        return false;
    }
    try {
        nlohmann::json sceneData = nlohmann::json::parse(jsonContent);
        if (globalEngine) {
            globalEngine->scene().serializer().deserializeScene(sceneData);
            return true;
        }
    } catch (const nlohmann::json::parse_error &e) {
        std::cerr << "[API-CATCH] Scene creation JSON parse error: " << e.what() << std::endl;
        return false;
    } catch (const std::runtime_error &e) {
        std::cerr << "[API-CATCH] Scene creation error: " << e.what() << std::endl;
        return false;
    } catch (const std::exception &e) {
        std::cerr << "[API-CATCH] Scene creation unknown error: " << e.what() << std::endl;
        return false;
    }
    return false;
}

bool scene_load_uuid(uint64_t sceneId) {
    try {
        if (globalEngine) {
            globalEngine->scene().loadScene(Hylozoa::UUID(sceneId));
        }
    } catch (const std::runtime_error &e) {
        std::cerr << "[API-CATCH] Scene load (uuid) error: " << e.what() << std::endl;
        return false;
    } catch (const std::exception &e) {
        std::cerr << "[API-CATCH] Scene load (uuid) unknown error: " << e.what() << std::endl;
        return false;
    }
    return false;
}

bool scene_load_name(const char* sceneName) {
    try {
        if (globalEngine) {
            globalEngine->scene().loadScene(sceneName);
        }
    } catch (const std::runtime_error &e) {
        std::cerr << "[API-CATCH] Scene load (name) error: " << e.what() << std::endl;
        return false;
    } catch (const std::exception &e) {
        std::cerr << "[API-CATCH] Scene load (name) unknown error: " << e.what() << std::endl;
        return false;
    }
    return false;
}

bool scene_unload_uuid(uint64_t sceneId) {
    try {
        if (globalEngine) {
            globalEngine->scene().unloadScene(Hylozoa::UUID(sceneId));
        }
    } catch (const std::runtime_error &e) {
        std::cerr << "[API-CATCH] Scene unload (uuid) error: " << e.what() << std::endl;
        return false;
    } catch (const std::exception &e) {
        std::cerr << "[API-CATCH] Scene unload (uuid) unknown error: " << e.what() << std::endl;
        return false;
    }
    return false;
}

bool scene_unload_name(const char* sceneName) {
    try {
        if (globalEngine) {
            globalEngine->scene().unloadScene(sceneName);
        }
    } catch (const std::runtime_error &e) {
        std::cerr << "[API-CATCH] Scene unload (name) error: " << e.what() << std::endl;
        return false;
    } catch (const std::exception &e) {
        std::cerr << "[API-CATCH] Scene unload (name) unknown error: " << e.what() << std::endl;
        return false;
    }
    return false;
}

// --------------------LAYER API FUNCTIONS IMPLEMENTATIONS--------------------