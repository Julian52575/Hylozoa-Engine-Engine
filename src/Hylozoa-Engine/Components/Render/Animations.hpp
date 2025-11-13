#pragma once
#include <string>
#include <unordered_map>

namespace Hylozoa {
    namespace Components {

        struct Animations {
            std::string currentAnimation;
            std::unordered_map<std::string, struct Animation> animations; // exemple -> animation[currentAnimation]
        };

        struct Animation {
            std::string animationName;
            float frameRate; // frames per second
            bool loop;
            float speed; // multiplier de la vitesse
        };
    }
}
