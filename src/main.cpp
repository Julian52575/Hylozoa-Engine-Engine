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
            "x": 412,
            "y": 241
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
          "texture": null,
          "scale": {
            "x": 1,
            "y": 1
          },
          "offset": {
            "x": 0.5,
            "y": 0.5
          }
        },
        "Camera": {
          "offset": {
            "x": 0,
            "y": 1.6
          },
          "anchorMode": "head",
          "viewportSize": {
            "x": 250.1,
            "y": 200.1
          },
          "zoom": 1.1,
          "order": 0,
          "isUI": false,
          "cullingMask": [
            "Default"
          ]
        },
        "Name": {
          "name": "Entity 1"
        }
      }
    }
  ]
})";

    engine_create("src/settings.json", false);
    engine_init();
    scene_create(jsonScene.c_str(), true);
    scene_load("Scene 1", false);
    // scene_destroy("Scene 1", false);
    // scene_load("Scene 1", false);
    auto layersJson = layer_list();
    std::cout << "Registered layers: " << layersJson << std::endl;
    delete[] layersJson;

    engine_run();
    engine_shutdown();

    return 0;
}
