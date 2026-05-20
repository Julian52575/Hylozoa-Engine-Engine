/*
** EPITECH PROJECT, 2026
** Hylozoa Engine
** File description:
** Tags related components serialization [header]
*/

#pragma once

#include "Tags.hpp"
#include "Hylozoa-Engine/Core/Tags/TagsManager.hpp"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace Hylozoa::Components {

inline void to_json(json &j, const Tags &tags) {
    TagsManager &tagsManager = TagsManager::instance();

    j = json{{"tags", tagsManager.serializeTags(tags.tags)}};
}

inline void from_json(const json &j, Tags &tags) {
    TagsManager &tagsManager = TagsManager::instance();
    const auto &tagNames = j.value("tags", std::vector<std::string>{});

    tags.tags = tagsManager.deserializeTags(tagNames);
}

}