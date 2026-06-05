/*
** EPITECH PROJECT, 2026
** Hylozoa Engine
** File description:
** Script related components serialization [header]
*/

#pragma once

#include "Script.hpp"

#include "Hylozoa-Engine/Core/jsonWrap.hpp"

namespace Hylozoa::Components {

inline void to_json(json &j, const Script &script) {
    j = json{{"scriptFile", script.scriptFile}};
}

inline void from_json(const json &j, Script &script) {
    script.scriptFile = j.value("scriptfile", "");
}

}