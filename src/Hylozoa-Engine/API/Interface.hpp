/*
** EPITECH PROJECT, 2026
** Hylozoa Engine
** File description:
** API functions prototypes
*/

#ifndef ENGINE_API_INTERFACE_HPP
#define ENGINE_API_INTERFACE_HPP

#include "Macros.hpp"
#include <cstdint>

extern "C" {

// --------------------ENGINE API FUNCTIONS PROTOTYPES-------------------
// SANS POINTEURS
API_EXPORT void engine_create(char *settingsPath);
API_EXPORT void engine_init(void);
API_EXPORT void engine_run(void);
API_EXPORT void engine_pause(void);
API_EXPORT void engine_stop(void);
API_EXPORT void engine_shutdown(void);

// --------------------SCENE API FUNCTIONS PROTOTYPES--------------------

API_EXPORT bool scene_create(const char *jsonContent);
API_EXPORT bool scene_destroy(uint64_t sceneId);
API_EXPORT bool scene_load_uuid(uint64_t sceneId);
API_EXPORT bool scene_load_name(const char* sceneName);
API_EXPORT bool scene_unload_uuid(uint64_t sceneId);
API_EXPORT bool scene_unload_name(const char* sceneName);
API_EXPORT const char* scene_list(void);

// --------------------LAYER API FUNCTIONS PROTOTYPES--------------------

API_EXPORT void layer_create(char *layerName);
API_EXPORT void layer_destroy(char *layerName);
API_EXPORT const char* layer_list(void);

}

#endif // ENGINE_API_INTERFACE_HPP