/*
** EPITECH PROJECT, 2026
** Hylozoa Engine
** File description:
** Listener related components serialization [header]
*/

#pragma once

#include "Listener.hpp"

#include "Hylozoa-Engine/Core/jsonWrap.hpp"

namespace Hylozoa::Components {

inline void to_json(json &j, const MainListener &mainListener) {
    j = json{};
}

inline void from_json(const json &j, MainListener &mainListener) {
    // No data to deserialize for MainListener
}

inline void to_json(json &j, const NoiseListener &noiseListener) {
    j = json{{"hearingRange", noiseListener.hearingRange}};
}

inline void from_json(const json &j, NoiseListener &noiseListener) {
    noiseListener.hearingRange = j.value("hearingrange", 100.0f);
}

}