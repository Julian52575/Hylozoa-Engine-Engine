//
// EPITECH PROJECT, 2025
// Hylozoa-Engine
// File description:
// main
//

#include "Hylozoa-Engine/Components/Components.hpp"
#include "Hylozoa-Engine/Core/Engine.hpp"
#include "Hylozoa-Engine/Core/Entities/Entity.hpp"
#include "Hylozoa-Engine/Core/Layers/LayerManager.hpp"
#include "Hylozoa-Engine/Placeholder/Placeholder.hpp"
#include <SDL3/SDL.h>
#include <entt/entt.hpp>
#include <iostream>

#include "Hylozoa-Engine/API/Interface.hpp"

inline std::string readFileToString(const std::string& filePath) {
    std::ifstream file(filePath);

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return std::string();
    }
    std::stringstream buffer;

    buffer << file.rdbuf();
    return buffer.str();
}

int main(int ac, char *const *av) {
    // std::cout << "Hello world from Hylozoa Game Engine main." << std::endl;

    // This demo will contain:
    
    // - sand, walkable
      // - a crab, walking on the sand
      // - a camera, following the crab
    // - water, not walkable
      // - a boat, floating on the water
      // - a camera, following the boat
    // - a palmtree that spaws coconuts
      // = coconuts fall on the sand
    const std::string jsonScene = readFileToString("Assets/beach.json");

    engine_create("src/settings.json", false);
    engine_init();
    scene_create(jsonScene.c_str(), true);
    scene_load("Scene 1", false);

    // auto* engine = get_engine_instance();

    // engine->scene().serializer().serializeScene(Hylozoa::UUID(8662741413288096373), "test_scene_output.hylozoa");
    // engine->runTick();
    // auto e = engine->getRegistry().view<Hylozoa::Components::Name>();
    // for (auto entity : e) {
    //   auto& name = e.get<Hylozoa::Components::Name>(entity);
    //   auto handle = Hylozoa::Entity::fromHandle(entity, engine->getRegistry());
    //   if (name.name == "Camera") {
    //       engine->audio().playNoise("audio/fire.wav", handle);
    //     }
    // }
    engine_run();

    auto* engine = get_engine_instance();

    engine->scene().serializer().serializeScene(Hylozoa::UUID(8662741413288096373), "test_scene_output.hylozoa");
    engine_stop();
    engine_shutdown();

    return 0;
}
