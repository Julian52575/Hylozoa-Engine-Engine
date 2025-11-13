#pragma once
#include <utility>

namespace Hylozoa {
    namespace Components {
        struct Transform2D {
            std::pair<float, float> position;
            float rotation; // degrees
            std::pair<float, float> scale;
        };
    }
}
