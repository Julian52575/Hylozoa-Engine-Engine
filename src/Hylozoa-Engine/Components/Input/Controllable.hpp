#pragma once
#include <SDL3/SDL.h>
#include <unordered_set>

namespace Hylozoa::Components {

struct Controllable {
    bool isControllable{true};
    std::unordered_set<SDL_Keycode> keysHeld; // toucches currently held down
};

} // namespace Hylozoa::Components
