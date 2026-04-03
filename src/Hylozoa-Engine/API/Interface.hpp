/*
** EPITECH PROJECT, 2026
** Hylozoa Engine
** File description:
** API functions prototypes
*/

#ifndef ENGINE_API_INTERFACE_HPP
#define ENGINE_API_INTERFACE_HPP

#include "Macros.hpp"

extern "C" {

// --------------------ENGINE API FUNCTIONS PROTOTYPES-------------------
// SANS POINTEURS
API_EXPORT void engine_create(char *settingsPath);
API_EXPORT void engine_init(void);
API_EXPORT void engine_run(void);
API_EXPORT void engine_pause(void);
API_EXPORT void engine_stop(void);
API_EXPORT void engine_shutdown(void);
// AVEC POINTEURS
API_EXPORT ENGINE_PTR engine_create_ptr(char *settingsPath);
API_EXPORT void engine_init_ptr(ENGINE_PTR engine);
API_EXPORT void engine_run_ptr(ENGINE_PTR engine);
API_EXPORT void engine_pause_ptr(ENGINE_PTR engine);
API_EXPORT void engine_stop_ptr(ENGINE_PTR engine);
API_EXPORT void engine_shutdown_ptr(ENGINE_PTR engine);

// --------------------SCENE API FUNCTIONS PROTOTYPES--------------------

// --------------------LAYER API FUNCTIONS PROTOTYPES--------------------

}

#endif // ENGINE_API_INTERFACE_HPP