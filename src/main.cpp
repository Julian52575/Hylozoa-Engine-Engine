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

    /*const std::string jsonScene = R"({
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
            "y": 84
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
            "x": 500,
            "y": 500
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
        }
      }
    }
  ]
})";*/


    // engine_create("src/settings.json", false);
    // engine_init();
    // scene_create(jsonScene.c_str(), true);
    // scene_load("Scene 1", false);
    // // scene_destroy("Scene 1", false);
    // // scene_load("Scene 1", false);
    // // auto layersJson = layer_list();
    // // std::cout << "Registered layers: " << layersJson << std::endl;
    // // delete[] layersJson;

    // engine_run();
    
    // engine_stop();
    // engine_shutdown();


    Hylozoa::Engine engine(Hylozoa::EngineMode::HEADLESS, "src/settings.json");
    engine.init();
    engine.scene().createScene("Scene 1");
    engine.scene().loadScene("Scene 1");
    auto test = engine.scene().spawnEntity("Test Entity");

    const std::string scriptTest = R"(
        function onUpdate(entity, dt)
          print("Hello from Lua! Delta time: " .. dt)
          if entity == nil then
            print("Entity is nil")
           else
            local transform = entity:get_transform()
            if transform == nil then
              print("Transform is nil")
            else
            local pos = transform.position
            print("X:", pos.x)
            pos.y = pos.y + 10 * dt
              -- print("Entity position: (" .. transform.position.x .. ", " .. transform.position.y .. ") ")
            end
          end
        end
    )";

    auto& scriptComponent = test.addComponent<Hylozoa::Components::Script>();

    engine.script().createScriptComponent(scriptComponent, scriptTest);

    engine.runTick(0.016f);

    auto& c = test.getComponent<Hylozoa::Components::LocalTransform>();
    std::cout << "Entity position after update: (" << c.position.x << ", " << c.position.y << ")\n";
    engine.shutdown();

    return 0;
}
