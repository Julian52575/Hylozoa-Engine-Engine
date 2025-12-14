/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Input Component [header]
*/

#ifndef INPUT_HPP_
#define INPUT_HPP_

#include <SDL3/SDL.h>
#include <array>

namespace Hylozoa::Components::HylozoaInternal {

/*
* @struct InputState
* @brief Component to store the state of input devices.
* 
* This component holds the current state of keyboard inputs
* such as keys pressed, released, and held down.
*/
struct InputState {
    std::array<bool, SDL_SCANCODE_COUNT> keysPressed;   // Keys pressed this frame
    std::array<bool, SDL_SCANCODE_COUNT> keysReleased;  // Keys released this frame
    std::array<bool, SDL_SCANCODE_COUNT> keysHeld;      // Keys currently held down
};

/*
* @struct MouseState
* @brief Component to store the state of mouse input.
* 
* This component holds the current state of mouse inputs
* such as position, buttons pressed, released, held down, and wheel movement.
*/
struct MouseState {
    int x{0};                                      // Current mouse x position
    int y{0};                                      // Current mouse y position
    std::array<bool, 8> buttonsPressed;            // Mouse buttons pressed this frame
    std::array<bool, 8> buttonsReleased;           // Mouse buttons released this frame
    std::array<bool, 8> buttonsHeld;               // Mouse buttons currently held down
    int wheelX{0};                                 // Mouse wheel x movement this frame
    int wheelY{0};                                 // Mouse wheel y movement this frame
};


}

#endif /* !INPUT_HPP_ */