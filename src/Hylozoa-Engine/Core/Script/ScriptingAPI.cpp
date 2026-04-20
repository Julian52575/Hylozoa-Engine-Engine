/*
** EPITECH PROJECT, 2026
** Hylozoa Engine
** File description:
** Scripting API [source]
*/

#include "ScriptingAPI.hpp"
#include "Hylozoa-Engine/Core/Settings.hpp"

#include <iostream>

namespace Hylozoa::Scripting::API {

void yolo() {
    std::cout << "YOLO from the hylozoa scripting API" << std::endl;
}

void log_message(const std::string& message) {
    std::cout << "[Script-log] " << message << std::endl;
}

Components::LocalTransform* get_transform(Entity& e) {
    try {
        return &e.getComponent<Components::LocalTransform>();
    } catch (const std::exception& ex) {
        if (Hylozoa::Settings::getInstance().getSettings().verbose) {
            std::cout << "[Script-API] Error in get_transform: " << ex.what() << std::endl;
        }
        return nullptr;
    }
}


}