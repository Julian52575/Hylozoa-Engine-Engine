#pragma once
#include <utility>

namespace Hylozoa {
    namespace Components {
        struct Collider {
            float width;
            float height;
            std::pair<float, float> offset; // x, y
        };
    }
}