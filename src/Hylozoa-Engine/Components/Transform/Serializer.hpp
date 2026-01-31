/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Transform related components serialization [header]
*/

#pragma once

#include "Transform.hpp"

#include <nlohmann/json.hpp>
// for convenience
using json = nlohmann::json;

namespace Hylozoa::Components {

inline void to_json(json &j, const Name &name) {
    j = json{{"name", name.name}};
}

inline void from_json(const json &j, Name &name) {
    name.name = j.value("name", "");
}

inline void to_json(json &j, const LocalTransform &lt) {
    j = json{{"position", {{"x", lt.position.x}, {"y", lt.position.y}}},
             {"scale", {{"x", lt.scale.x}, {"y", lt.scale.y}}},
             {"rotation", lt.rotation}};
}

inline void from_json(const json &j, LocalTransform &lt) {
    const auto &pos = j.value("position", json::object());
    lt.position.x = pos.value("x", 0.0f);
    lt.position.y = pos.value("y", 0.0f);

    const auto &scale = j.at("scale");
    lt.scale.x = scale.value("x", 1.0f);
    lt.scale.y = scale.value("y", 1.0f);

    lt.rotation = j.value("rotation", 0.0f);
}

inline void to_json(json &j, const WorldTransform &wt) {
    j = json{{"position", {"x", wt.position.x}, {"y", wt.position.y}},
             {"scale", {"x", wt.scale.x}, {"y", wt.scale.y}},
             {"rotation", wt.rotation}};
}

inline void from_json(const json &j, WorldTransform &wt) {
    const auto &pos = j.value("position", json::object());
    wt.position.x = pos.value("x", 0.0f);
    wt.position.y = pos.value("y", 0.0f);

    const auto &scale = j.value("scale", json::object());
    wt.scale.x = scale.value("x", 1.0f);
    wt.scale.y = scale.value("y", 1.0f);

    wt.rotation = j.value("rotation", 0.0f);
}

} // namespace Hylozoa::Components