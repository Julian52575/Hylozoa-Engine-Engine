/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Renderer related components serialization [header]
*/

#pragma once

#include "Hylozoa-Engine/Core/Layers/LayerManager.hpp"
#include "Renderable.hpp"

#include <nlohmann/json.hpp>
// for convenience
using json = nlohmann::json;

namespace Hylozoa::Components::Rendering {

inline void to_json(json &j, const Renderable &r) {
    j = json{{"color",
              {{"r", r.color.r},
               {"g", r.color.g},
               {"b", r.color.b},
               {"a", r.color.a}}},
             {"visible", r.visible},
             {"layer", LayerManager::instance().getLayerNameByBit(r.layer)},
             {"transparency", r.transparency},
             {"origin", {{"x", r.origin.x}, {"y", r.origin.y}}}};
}

inline void from_json(const json &j, Renderable &r) {
    const auto &color = j.value("color", json::object());
    r.color.r = color.value("r", 255);
    r.color.g = color.value("g", 255);
    r.color.b = color.value("b", 255);
    r.color.a = color.value("a", 255);

    r.visible = j.value("visible", true);

    std::string layerName = j.value("layer", "Default");
    r.layer = LayerManager::instance().getLayerBitByName(layerName);

    r.transparency = j.value("transparency", 1.0f);
    r.origin.x = j.value("origin", json::object()).value("x", 0.5f);
    r.origin.y = j.value("origin", json::object()).value("y", 0.5f);
}

inline void to_json(json &j, const RenderableShape &rs) {
    json specsJson;

    if (rs.type == RenderableShape::ShapeType::Rectangle) {
        const auto &rectSpecs =
            std::get<RenderableShape::RectangleSpecs>(rs.specs);
        specsJson =
            json{{"width", rectSpecs.width}, {"height", rectSpecs.height}};
    } else if (rs.type == RenderableShape::ShapeType::Circle) {
        const auto &circleSpecs =
            std::get<RenderableShape::CircleSpecs>(rs.specs);
        specsJson = json{{"radius", circleSpecs.radius}};
    }

    j = json{{"type", rs.type == RenderableShape::ShapeType::Rectangle
                          ? "Rectangle"
                          : "Circle"},
             {"specs", specsJson},
             {"outlineColor",
              {{"r", rs.outlineColor.r},
               {"g", rs.outlineColor.g},
               {"b", rs.outlineColor.b},
               {"a", rs.outlineColor.a}}},
             {"outlineThickness", rs.outlineThickness}};
}

inline void from_json(const json &j, RenderableShape &rs) {
    std::string typeStr = j.value("type", "Rectangle");

    if (typeStr == "Rectangle" || typeStr == "rectangle") {
        rs.type = RenderableShape::ShapeType::Rectangle;
        const auto &specsJson = j.value("specs", json::object());

        RenderableShape::RectangleSpecs rectSpecs;

        rectSpecs.width = specsJson.value("width", 30.0f);
        rectSpecs.height = specsJson.value("height", 30.0f);
        rs.specs = rectSpecs;
    } else if (typeStr == "Circle" || typeStr == "circle") {
        rs.type = RenderableShape::ShapeType::Circle;
        const auto &specsJson = j.value("specs", json::object());

        RenderableShape::CircleSpecs circleSpecs;

        circleSpecs.radius = specsJson.value("radius", 30.0f);
        rs.specs = circleSpecs;
    }

    const auto &outlineColor = j.value("outlineColor", json::object());
    rs.outlineColor.r = outlineColor.value("r", 0);
    rs.outlineColor.g = outlineColor.value("g", 0);
    rs.outlineColor.b = outlineColor.value("b", 0);
    rs.outlineColor.a = outlineColor.value("a", 255);

    rs.outlineThickness = j.value("outlineThickness", 1.0f);
}

inline void to_json(json &j, const Sprite &s) {
    j = json{{
                 "texture",
                 s.textureName,
             },
             {"scale", {{"x", s.scale.x}, {"y", s.scale.y}}}};
}

inline void from_json(const json &j, Sprite &s) {
    if (j["texture"].is_null()) {
        s.textureName = "";
    } else {
        s.textureName = j.value("texture", "");
    }

    const auto &scale = j.value("scale", json::object());
    s.scale.x = scale.value("x", 1.0f);
    s.scale.y = scale.value("y", 1.0f);
}

} // namespace Hylozoa::Components::Rendering