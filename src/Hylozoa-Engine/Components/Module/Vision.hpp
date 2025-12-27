#pragma once
#include <utility>
#include <vector>
#include <entt/entt.hpp>

namespace Hylozoa::Components::Module {
    struct Vision
    {
        std::pair<float, float> offset; //position relative to entity position
        float fov; // field of view in degrees
        float range; // maximum distance of vision
        float direction; // direction the vision is facing
        int rayCount; // number of rays to cast for vision

        std::vector<entt::entity> visibleEntities; //list of currently visible entities

        //debug
        bool debugDraw = true;
    };
}