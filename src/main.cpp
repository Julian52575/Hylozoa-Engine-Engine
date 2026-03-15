//
// EPITECH PROJECT, 2025
// Hylozoa-Engine
// File description:
// main
//

#include "Hylozoa-Engine/Components/Components.hpp"
#include "Hylozoa-Engine/Core/Engine.hpp"
#include "Hylozoa-Engine/Core/Entity.hpp"
#include "Hylozoa-Engine/Core/LayerManager.hpp"
#include "Hylozoa-Engine/Placeholder/Placeholder.hpp"
#include "Hylozoa-Engine/Systems/HelloWorld/HelloWorld.hpp"
#include <SDL3/SDL.h>
#include <entt/entt.hpp>
#include <iostream>

int main(int ac, char *const *av) {

    // std::cout << "Hello world from Hylozoa Game Engine main." << std::endl;

    Hylozoa::Engine engine;
    Hylozoa::Components::Rendering::Renderable renderable;
    // renderable.color = {255, 255, 255, 0};

    // auto ground = engine.scene().spawnEntity("Ground");
    auto player = engine.scene().spawnEntity("Player");
    auto camera = engine.scene().spawnEntity("Main Camera");
    // camera.childOf(player);
    camera.addComponent<Hylozoa::Components::Camera>();
    camera.addTagComponent<Hylozoa::Components::MainListener>();
    player.addComponent<Hylozoa::Components::NoiseListener>();
    player.addComponent<Hylozoa::Components::WorldTransform>();

    // player.addComponent<Hylozoa::Components::Rendering::Sprite>(Hylozoa::Components::Rendering::Sprite{"textures/missing.png", {1.0f, 1.0f}, {0, 0}});
    // player.addComponent<Hylozoa::Components::HylozoaInternal::RenderTexture>();


    // auto &circle =
    //     player.addComponent<Hylozoa::Components::CircleColliderComponent>();
    // circle.radius = 100.f;

    renderable.color = {0, 0, 255, 255};
    renderable.layer =
    Hylozoa::LayerManager::instance().getLayerBitByName("Default");

    player.getComponent<Hylozoa::Components::LocalTransform>().position = {
        0.0f, 0.0f};
    player.addComponent<Hylozoa::Components::Rendering::Renderable>(renderable);
    // player.addComponent<Hylozoa::Components::RigidBodyComponent>().type =
    //     b2_dynamicBody;
    // player.addComponent<Hylozoa::Components::ColliderComponent>()
    //     .enableContactEvents = true;
    player.addComponent<Hylozoa::Components::Rendering::RenderableShape>(
        Hylozoa::Components::Rendering::RenderableShape{
            Hylozoa::Components::Rendering::RenderableShape::ShapeType::Circle,
            Hylozoa::Components::Rendering::RenderableShape::CircleSpecs{
                100.0f}});
    // player.addComponent<Hylozoa::Components::Controllable>();

    // auto &boxGround =
    //     ground.addComponent<Hylozoa::Components::BoxColliderComponent>();
    // boxGround.halfWidth = 500.f;
    // boxGround.halfHeight = 50.f;

    // renderable.color = {0, 255, 0, 255};
    // ground.getComponent<Hylozoa::Components::LocalTransform>().position = {
    //     0.0f, 300.0f};
    // ground.addComponent<Hylozoa::Components::RigidBodyComponent>().type =
    //     b2_staticBody;
    // ground.addComponent<Hylozoa::Components::ColliderComponent>()
    //     .enableContactEvents = true;
    // ground.addComponent<Hylozoa::Components::Rendering::Renderable>(renderable);
    // ground.addComponent<Hylozoa::Components::Rendering::RenderableShape>(
    //     Hylozoa::Components::Rendering::RenderableShape{
    //         Hylozoa::Components::Rendering::RenderableShape::ShapeType::
    //             Rectangle,
    //         Hylozoa::Components::Rendering::RenderableShape::RectangleSpecs{
    //             boxGround.halfWidth * 2, boxGround.halfHeight * 2}});

    // std::cout << "Player entity: " << player.getName(engine) << std::endl;

    // engine.runTick(90);
    //   engine.runTick(1);

    // engine.scene().unloadScene("DefaultScene");

    // engine.scene().serializer().deserializeScene("scene_save.hylozoa");
    // engine.scene().loadScene(Hylozoa::UUID(3823173490725659066));
    // engine.scene().serializer().serializeScene(
    //     Hylozoa::UUID(3817794012907643580), "scene_saveout.hylozoa");
    // engine.scene().serializer().serializeScene(camera.getComponent<Hylozoa::Components::HylozoaInternal::SceneTag>().id,
    // "scene_save.hylozoa");
    auto soundPoint = engine.scene().spawnEntity("Sound Point");
    soundPoint.getComponent<Hylozoa::Components::LocalTransform>().position = {100.0f, 0.0f};
    soundPoint.addComponent<Hylozoa::Components::WorldTransform>().position = {100.0f, 0.0f};
    engine.audio().playNoise("audio/fire.wav", soundPoint);
    // engine.run();
    engine.runTick(10);
    return 0;
}
