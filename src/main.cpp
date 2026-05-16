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

int main(int ac, char *const *av) {

    // std::cout << "Hello world from Hylozoa Game Engine main." << std::endl;

    const std::string jsonScene = R"({
  "sceneID": "8662741413288096373",
  "sceneName": "Scene 1",
  "Entities": [
    {
      "UUID": "525500062871416371",
      "Parent": null,
      "Components": {
        "LocalTransform": {
          "position": {
            "x": 10,
            "y": 10
          },
          "scale": {
            "x": 1,
            "y": 1
          },
          "rotation": 0
        },
        "Renderable": {
          "color": {
            "r": 255,
            "g": 0,
            "b": 0,
            "a": 255
          },
          "visible": true,
          "LayerBit": "Default",
          "transparency": 1,
          "origin": {
            "x": 0.5,
            "y": 0.5
          }
        },
        "Sprite": {
          "texture": "textures/camea.png",
          "scale": {
            "x": 1,
            "y": 1
          },
          "offset": {
            "x": 0.5,
            "y": 0.5
          }
        },
        "Script": {
          "scriptFile": "scripts/test.lua"
        },
        "Name": {
          "name": "Entity 1"
        }
      }
    },
    {
      "UUID": "846548945688888845",
      "Parent": null,
      "Components": {
        "LocalTransform": {
          "position": {
            "x": 0,
            "y": 0
          },
          "scale": {
            "x": 1,
            "y": 1
          },
          "rotation": 0
        },
        "Camera": {
          "offset": {
            "x": 0,
            "y": 1.6
          },
          "viewportSize": {
            "x": 1920,
            "y": 1080
          },
          "zoom": 1.1,
          "order": 0,
          "isUI": false,
          "cullingMask": [
            "Default"
          ]
        },
        "Name": {
          "name": "Camera"
        },
        "NoiseListener": {
          "hearingRange": 100
        }
      }
    }
  ]
})";


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
