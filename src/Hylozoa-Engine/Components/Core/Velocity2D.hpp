#pragma once
#include <utility>

namespace Hylozoa {
    namespace Components {
        struct Velocity2D {
            std::pair<float, float> velocity; // x and y components of velocity
        };
    }
}