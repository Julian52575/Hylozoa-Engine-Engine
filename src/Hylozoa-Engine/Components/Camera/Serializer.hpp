/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Camera related components serialization [header]
*/

#pragma once

#include "Camera.hpp"
#include "Hylozoa-Engine/Core/Layers/LayerManager.hpp"

#include "Hylozoa-Engine/Core/jsonWrap.hpp"

namespace Hylozoa::Components {

inline void to_json(json &j, const Camera &camera) {
    LayerManager &layerManager = LayerManager::instance();
    j = json{{"viewportSize",
             {{"x", camera.viewportSize.x}, {"y", camera.viewportSize.y}}},
             {"offset", {{"x", camera.offset.x}, {"y", camera.offset.y}}},
             {"zoom", camera.zoom},
             {"order", camera.order},
             {"cullingMask", layerManager.maskToNames(camera.cullingMask)},
             {"isUI", camera.isUI}};
}

inline void from_json(const json &j, Camera &camera) {
    const auto &viewportSize = j.value("viewportsize", json::object());
    camera.viewportSize.x = viewportSize.value("x", 1920.0f);
    camera.viewportSize.y = viewportSize.value("y", 1080.0f);

    const auto &offset = j.value("offset", json::object());
    camera.offset.x = j.value("offset", json::object()).value("x", 0.0f);
    camera.offset.y = j.value("offset", json::object()).value("y", 0.0f);

    camera.zoom = j.value("zoom", 1.0f);
    camera.order = j.value("order", 0);
    camera.isUI = j.value("isui", false);

    LayerManager &layerManager = LayerManager::instance();
    if (j.contains("cullingmask")) {
        const auto &cullingMaskNames =
            j.value("cullingmask", std::vector<std::string>{});
        camera.cullingMask = layerManager.buildMask(cullingMaskNames);
        for (const auto &name : cullingMaskNames) {
            if (!layerManager.hasLayer(name)) {
                std::cerr << "Warning: Layer '" << name
                          << "' does not exist. It will be ignored in the "
                             "culling mask.\n";
            } else {
                std::cout << "Layer '" << name
                          << "' added to the culling mask.\n";
            }

        }
    }
}

} // namespace Hylozoa::Components
