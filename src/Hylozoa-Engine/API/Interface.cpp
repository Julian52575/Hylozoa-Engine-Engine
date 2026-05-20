/*
** EPITECH PROJECT, 2026
** Hylozoa Engine
** File description:
** API functions implementations
*/

#include <format>
#include <fstream>
#include <sstream>

#include "nlohmann/json.hpp"

#include "Hylozoa-Engine/Components/Scene/UUID.hpp"
#include "Hylozoa-Engine/Core/Engine.hpp"
#include "Hylozoa-Engine/Core/Layers/LayerManager.hpp"
#include "Hylozoa-Engine/SDL/SDL_Manager.hpp"
#include "Interface.hpp"

static Hylozoa::Engine *globalEngine = nullptr;

Hylozoa::Engine* get_engine_instance() {
    return globalEngine;
}

// --------------------ENGINE API FUNCTIONS IMPLEMENTATIONS-------------------

API_EXPORT void engine_create(const char *settings, bool isRaw) {
    if (globalEngine != nullptr) {
        return;
    }
    if (settings == nullptr) {
        globalEngine = new Hylozoa::Engine(Hylozoa::EngineMode::NORMAL);
        return;
    }
    if (isRaw) {
        std::istringstream jsonStream(settings);

        globalEngine = new Hylozoa::Engine(Hylozoa::EngineMode::NORMAL, jsonStream);
        return;
    }

    globalEngine = new Hylozoa::Engine(settings);
}

API_EXPORT void engine_init() {
    if (globalEngine) {
        globalEngine->init();
    }
}

API_EXPORT void engine_run() {
    if (globalEngine) {
        globalEngine->run();
    }
}

API_EXPORT void engine_pause() {
    if (globalEngine) {
        globalEngine->pause();
    }
}

API_EXPORT void engine_unpause() {
    if (globalEngine) {
        globalEngine->unpause();
    }
}

API_EXPORT void engine_stop() {
    if (globalEngine) {
        globalEngine->stop();
    }
}

API_EXPORT void engine_shutdown() {
    if (not globalEngine) {
        return;
    }
    globalEngine->stop();
    globalEngine->shutdown();
    delete globalEngine;
    globalEngine = nullptr;
    Hylozoa::SDL::SDL_Manager::getInstance().shutdown();
}

// --------------------SCENE API FUNCTIONS IMPLEMENTATIONS--------------------

API_EXPORT bool scene_create(const char *sceneData, bool isRaw) {
    if (sceneData == nullptr) {
        return false;
    }
    try {
        nlohmann::json sceneJson;
        if (isRaw) {
            sceneJson = nlohmann::json::parse(sceneData);
        } else {
            std::ifstream file(sceneData);

            if (!file.is_open()) {
                std::cerr << "[API-ERROR] Could not open scene file: " << sceneData << std::endl;
                return false;
            }
            sceneJson = nlohmann::json::parse(file);
        }

        if (globalEngine) {
            globalEngine->scene().serializer().deserializeScene(sceneJson);
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

API_EXPORT bool scene_destroy_uuid(uint64_t sceneId) {
    try {
        if (globalEngine) {
            globalEngine->scene().destroyScene(Hylozoa::UUID(sceneId));
        }
    } catch (const std::runtime_error &e) {
        std::cerr << "[API-CATCH] Scene destroy (uuid) error: " << e.what() << std::endl;
        return false;
    } catch (const std::exception &e) {
        std::cerr << "[API-CATCH] Scene destroy (uuid) unknown error: " << e.what() << std::endl;
        return false;
    }
    return false;
}

API_EXPORT bool scene_destroy_name(const char *sceneName) {
    try {
        if (globalEngine) {
            globalEngine->scene().destroyScene(sceneName);
        }
    } catch (const std::runtime_error &e) {
        std::cerr << "[API-CATCH] Scene destroy (name) error: " << e.what() << std::endl;
        return false;
    } catch (const std::exception &e) {
        std::cerr << "[API-CATCH] Scene destroy (name) unknown error: " << e.what() << std::endl;
        return false;
    }
    return false;
}

API_EXPORT bool scene_destroy(const char *scene, bool isUUID) {
    if (isUUID) {
        try {
            uint64_t sceneId = std::stoull(scene);
            return scene_destroy_uuid(sceneId);
        } catch (const std::invalid_argument &e) {
            std::cerr << "[API-CATCH] Scene destroy (uuid) invalid argument: " << e.what() << std::endl;
            return false;
        } catch (const std::out_of_range &e) {
            std::cerr << "[API-CATCH] Scene destroy (uuid) out of range: " << e.what() << std::endl;
            return false;
        }
    } else {
        return scene_destroy_name(scene);
    }
}

API_EXPORT bool scene_load_uuid(uint64_t sceneId) {
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

API_EXPORT bool scene_load_name(const char *sceneName) {
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

API_EXPORT bool scene_load(const char *scene, bool isUUID) {
    if (isUUID) {
        try {
            uint64_t sceneId = std::stoull(scene);
            return scene_load_uuid(sceneId);
        } catch (const std::invalid_argument &e) {
            std::cerr << "[API-CATCH] Scene load (uuid) invalid argument: " << e.what() << std::endl;
            return false;
        } catch (const std::out_of_range &e) {
            std::cerr << "[API-CATCH] Scene load (uuid) out of range: " << e.what() << std::endl;
            return false;
        }
    } else {
        return scene_load_name(scene);
    }
}

API_EXPORT bool scene_unload_uuid(uint64_t sceneId) {
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

API_EXPORT bool scene_unload_name(const char *sceneName) {
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

API_EXPORT bool scene_unload(const char *scene, bool isUUID) {
    if (isUUID) {
        try {
            uint64_t sceneId = std::stoull(scene);
            return scene_unload_uuid(sceneId);
        } catch (const std::invalid_argument &e) {
            std::cerr << "[API-CATCH] Scene unload (uuid) invalid argument: " << e.what() << std::endl;
            return false;
        } catch (const std::out_of_range &e) {
            std::cerr << "[API-CATCH] Scene unload (uuid) out of range: " << e.what() << std::endl;
            return false;
        }
    } else {
        return scene_unload_name(scene);
    }
}

// --------------------LAYER API FUNCTIONS IMPLEMENTATIONS--------------------

API_EXPORT void layer_create(const char *layerName) {
    if (globalEngine) {
        try {
            Hylozoa::LayerManager::instance().registerLayer(layerName);
        } catch (const std::runtime_error &e) {
            std::cerr << "[API-CATCH] Layer create error: " << e.what() << '\n';
        } catch (const std::exception &e) {
            std::cerr << "[API-CATCH] Layer create unknown error: " << e.what()
                      << '\n';
        }
    }
}

API_EXPORT void layer_destroy(const char *layerName) {
    if (globalEngine) {
        try {
            Hylozoa::LayerManager::instance().unregisterLayer(layerName);
        } catch (const std::runtime_error &e) {
            std::cerr << "[API-CATCH] Layer destroy error: " << e.what()
                      << '\n';
        } catch (const std::exception &e) {
            std::cerr << "[API-CATCH] Layer destroy unknown error: " << e.what()
                      << '\n';
        }
    }
}

API_EXPORT const char *layer_list() {
    if (globalEngine) {
        try {
            const auto &layers = Hylozoa::LayerManager::instance().layers();
            nlohmann::json jsonLayers = nlohmann::json::array();

            for (const auto &[name, bit] : layers) {
                jsonLayers.push_back(name);
            }

            std::string jsonString = jsonLayers.dump();
            char *result = new char[jsonString.size() + 1];
            std::memcpy(result, jsonString.c_str(), jsonString.size() + 1);
            return result;
        } catch (const std::runtime_error &e) {
            std::cerr << "[API-CATCH] Layer list error: " << e.what() << '\n';
        } catch (const std::exception &e) {
            std::cerr << "[API-CATCH] Layer list unknown error: " << e.what()
                      << '\n';
        }
    }
    return nullptr;
}


API_EXPORT void free_string(const char* str) {
    delete[] str;
}


// --------------------UTILITY API FUNCTIONS IMPLEMENTATIONS--------------------

API_EXPORT void generate_uuid(char* outPtr, size_t size) {
    if (outPtr == nullptr || size == 0) {
        return;
    }

    uint64_t uuid = Hylozoa::UUID();

    auto result = std::format_to_n(outPtr, size - 1, "{}", uuid);

    *result.out = '\0';
}


API_EXPORT void reload_settings_file(const char* settingsPath)
{
    if (!globalEngine) return;

    try {
        globalEngine->reloadSettings(settingsPath);
    }  catch(const std::runtime_error &e) {
        std::cerr << "[API-CATCH] Reload settings error: " << e.what() << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "[API-CATCH] Reload settings unknown error: " << e.what() << std::endl;
    }
}

API_EXPORT void reload_settings_json(const char* settingsJson)
{
    if (!globalEngine) return;
    
    try {
        json jsonStream = nlohmann::json::parse(settingsJson);
    
        globalEngine->reloadSettings(jsonStream);
    } catch(const std::runtime_error &e) {
        std::cerr << "[API-CATCH] Reload settings error: " << e.what() << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "[API-CATCH] Reload settings unknown error: " << e.what() << std::endl;
    }

}

API_EXPORT void reload_settings(const char* settings, bool isRaw)
{
    if (isRaw) {
        reload_settings_json(settings);
    } else {
        reload_settings_file(settings);
    }
}
