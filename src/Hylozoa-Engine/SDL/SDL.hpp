//
// EPITECH PROJECT, 2025
// Hylozoa-Engine
// File description:
// SDL header
//
#include <SDL3/SDL.h>
// #include <SDL3/SDL_main.h>

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]);
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event);
SDL_AppResult SDL_AppIterate(void *appstate);
void SDL_AppQuit(void *appstate, SDL_AppResult result);