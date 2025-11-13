#pragma once
#include <string>

namespace Hylozoa {
    namespace Components {

        struct SpriteRenderer {
            std::string texturePath; //ou texture directe
            int Zindex;
            std::tuple<int, int> size; //width, height
            std::tuple<int, int,int,int> color; //r, g, b, a
        };
    }
}