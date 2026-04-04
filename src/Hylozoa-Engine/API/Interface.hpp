/*
** EPITECH PROJECT, 2026
** Hylozoa Engine
** File description:
** API functions prototypes
*/

#ifndef ENGINE_API_INTERFACE_HPP
#define ENGINE_API_INTERFACE_HPP
/**
 * @file Interface.hpp
 * @brief Defines the API functions for the Hylozoa Engine.
 * 
 * This header file declares the functions that can be called from the UI to interact with the engine,
 * such as creating an engine instance, initializing it, running the main loop, and managing scenes and layers.
 * The functions are designed to be exported for use in dynamic libraries.
 */

#include "Macros.hpp"
#include <cstdint>

extern "C" {

// --------------------ENGINE API FUNCTIONS PROTOTYPES-------------------
/**
 * @brief creates an instance of the engine
 * 
 * This function should be called before any other engine function to initialize the engine instance.
 * 
 * @param settingsPath path to a settings json file to load settings.
 */
API_EXPORT void engine_create(const char *settingsPath);
/**
 * @brief initializes the engine
 * 
 * This function initializes the engine, setting up all necessary components and systems.
 * It should be called after engine_create() and before engine_run().
 */
API_EXPORT void engine_init(void);
/**
 * @brief run the engine main loop
 * 
 * This function starts the main loop of the engine, allowing it to process updates, render scenes, and handle events.
 * It should be called after engine_init().
 */
API_EXPORT void engine_run(void);
/**
 * @brief pauses the engine
 * 
 * This function pauses the engine, halting all updates and rendering. It can be used to temporarily stop the engine without stoping it completely.
 */
API_EXPORT void engine_pause(void);
/**
 * @brief unpauses the engine
 * 
 * This function unpauses the engine, resuming all updates and rendering after it has been paused.
 */
API_EXPORT void engine_unpause(void);
/**
 * @brief stops the engine
 * 
 * This function stops the engine, halting all updates and rendering.
 * It can be used to stop the engine without stoping the main loop completely, allowing for restarting.
 */
API_EXPORT void engine_stop(void);
/**
 * @brief shuts down the engine and releases all resources
 * 
 * This function should be called when the engine is no longer needed to clean up resources and properly shut down the engine instance.
 */
API_EXPORT void engine_shutdown(void);

// --------------------SCENE API FUNCTIONS PROTOTYPES--------------------

/**
 * @brief create a scene from a JSON string
 * 
 * This function creates a new scene in the engine using the provided JSON string, which should contain all necessary data to define the scene, including entities, components, and their relationships. The JSON format should follow the structure expected by the engine's scene management system. The function returns true if the scene was successfully created, or false if there was an error during creation (e.g., invalid JSON format, missing required fields, etc.).
 * 
 * @param jsonContent a JSON string containing the scene data to create.
 */
API_EXPORT bool scene_create(const char *jsonContent);
/**
 * @brief destroy a scene by its UUID
 * 
 * This function destroys a scene in the engine using its UUID.
 * It removes the scene and all associated entities from the engine's scene management system.
 * The function returns true if the scene was successfully destroyed, or false if there was an error (e.g., scene not found, invalid UUID, etc.).
 * 
 * @param sceneId the UUID of the scene to destroy.
 */
API_EXPORT bool scene_destroy_uuid(uint64_t sceneId);
/**
 * @brief destroy a scene by its name
 * 
 * This function destroys a scene in the engine using its UUID.
 * It removes the scene and all associated entities from the engine's scene management system.
 * The function returns true if the scene was successfully destroyed, or false if there was an error (e.g., scene not found, invalid UUID, etc.).
 * 
 * @param sceneName the name of the scene to destroy.
 */
API_EXPORT bool scene_destroy_name(const char* sceneName);
/**
 * @brief load a scene by its UUID
 * 
 * This function loads a scene in the engine using its UUID.
 * It activates the scene, making it the current active scene in the engine, and allows it to be updated and rendered.
 * 
 * @param sceneId the UUID of the scene to load.
 */
API_EXPORT bool scene_load_uuid(uint64_t sceneId);
/**
 * @brief load a scene by its name
 * 
 * This function loads a scene in the engine using its name.
 * It activates the scene, making it the current active scene in the engine, and allows it to be updated and rendered.
 * 
 * @param sceneName the name of the scene to load.
 */
API_EXPORT bool scene_load_name(const char* sceneName);
/**
 * @brief unload a scene by its UUID
 * 
 * This function unloads a scene in the engine using its UUID.
 * It deactivates the scene, making it no longer the current active scene in the engine, and prevents it from being updated and rendered.
 * 
 * @param sceneId the UUID of the scene to unload.
 */
API_EXPORT bool scene_unload_uuid(uint64_t sceneId);
/**
 * @brief unload a scene by its name
 * 
 * This function unloads a scene in the engine using its name.
 * It deactivates the scene, making it no longer the current active scene in the engine, and prevents it from being updated and rendered.
 * 
 * @param sceneName the name of the scene to unload.
 */
API_EXPORT bool scene_unload_name(const char* sceneName);
/**
 * @brief returns a list of all registered scenes in the engine
 * 
 * This function retrieves a list of all scenes that are currently registered in the engine, including both loaded and unloaded scenes.
 * The list is returned as a JSON string, which contains an array of scene objects, each with its UUID, name, and other relevant information.
 * This allows the caller to easily access and display the available scenes in the engine.
 * 
 * @return const char* a JSON string containing a list of all registered scenes in the engine.
 */
API_EXPORT const char* scene_list(void);

// --------------------LAYER API FUNCTIONS PROTOTYPES--------------------

API_EXPORT void layer_create(char *layerName);
API_EXPORT void layer_destroy(char *layerName);
API_EXPORT const char* layer_list(void);

// --------------------UTILITY API FUNCTIONS PROTOTYPES--------------------

API_EXPORT const char* engine_version(void);
/**
 * @brief generates a new UUID
 * 
 * This function generates a new unique identifier (UUID) that can be used to identify entities or scenes within the engine.
 * The UUID is returned as a 64-bit unsigned integer.
 * 
 * @return uint64_t - the generated UUID.
 */
API_EXPORT uint64_t generate_uuid(void);

}

#endif // ENGINE_API_INTERFACE_HPP