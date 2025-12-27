//
// EPITECH PROJECT, 2025
// Hylozoa-Engine
// File description:
// main
//

#include "Hylozoa-Engine/Components/Physics/Physics.hpp"
#include "Hylozoa-Engine/Components/Rendering/Renderable.hpp"
#include "Hylozoa-Engine/Components/Transform/Transform.hpp"
#include "Hylozoa-Engine/Components/Input/Controllable.hpp"
#include "Hylozoa-Engine/Core/Engine.hpp"
#include "Hylozoa-Engine/Core/Entity.hpp"
#include "Hylozoa-Engine/Placeholder/Placeholder.hpp"
#include <SDL3/SDL.h>
#include <entt/entt.hpp>
#include <iostream>

#include "Hylozoa-Engine/Components/Module/Vision.hpp"

typedef struct RayCastCallback {
    b2BodyId hitBody;
    void* userData;
    b2Vec2 hitPoint;
} RayCastCallback;

struct VisionReport {
    // Clé : l'entité vue, Valeur : liste des points d'impact sur cette entité
    std::unordered_map<entt::entity, std::vector<b2Vec2>> seenEntities;
};

float rayCastCB(b2ShapeId shapeId, b2Vec2 point, b2Vec2 normal, float fraction, void* context) {
    RayCastCallback* callback = (RayCastCallback*)context;

    callback->hitBody = b2Shape_GetBody(shapeId);;
    callback->userData = b2Shape_GetUserData(shapeId);
    callback->hitPoint = point;

    return fraction;
}


std::unordered_map<entt::entity, std::vector<b2Vec2>> updateVision(
  entt::entity observer, 
  b2WorldId world, 
  b2Vec2 pos, 
  float dirDegrees, 
  float fovDegrees, 
  float range, 
  int rayCount
) {
  std::unordered_map<entt::entity, std::vector<b2Vec2>> visionMap;

  float dirRad = dirDegrees * (M_PI / 180.0f);
  float fovRad = fovDegrees * (M_PI / 180.0f);
  float startAngle = dirRad - (fovRad / 2.0f);
  float angleStep = fovRad / (rayCount - 1);

  for (int i = 0; i < rayCount; ++i) {
    float currentAngle = startAngle + (i * angleStep);

    b2Vec2 start = pos;
    b2Vec2 end = {
      pos.x + range * cosf(currentAngle),
      pos.y + range * sinf(currentAngle)
    };
    RayCastCallback callback = {b2_nullBodyId,nullptr};
    b2World_CastRay(world, start, end,b2DefaultQueryFilter(), rayCastCB, &callback);

    if (B2_IS_NON_NULL(callback.hitBody)) {
        auto hitEntity = static_cast<entt::entity>(reinterpret_cast<uintptr_t>(callback.userData));
        if (hitEntity != observer) {
          visionMap[hitEntity].push_back(callback.hitPoint);
        }
    }
  }
  return visionMap;
}


int main(int ac, char *const *av) {

  std::cout << "Hello world from Hylozoa Game Engine main." << std::endl;

  Hylozoa::Engine engine;
  Hylozoa::Components::Rendering::Renderable renderable;
  renderable.color = {255, 255, 255, 0};

  auto weapon = engine.createSpacialEntity("Weapon");
  auto camera = engine.createSpacialEntity("Camera");
  auto player = engine.createSpacialEntity("Player");
  auto ground = engine.createSpacialEntity("ground");



  weapon.addComponent<Hylozoa::Components::Rendering::Renderable>(renderable);
  weapon.addComponent<Hylozoa::Components::Rendering::RenderableShape>(
    Hylozoa::Components::Rendering::RenderableShape{
          Hylozoa::Components::Rendering::RenderableShape::ShapeType::Circle,
          Hylozoa::Components::Rendering::RenderableShape::CircleSpecs{50.0f}
    }
  );
  weapon.childOf(player);


  camera.addComponent<Hylozoa::Components::Rendering::Renderable>(renderable);
  camera.addComponent<Hylozoa::Components::Rendering::RenderableTexture>(
    Hylozoa::Components::Rendering::RenderableTexture{
          "assets/textures/missing.png"
    }
  );
  camera.childOf(player);

  
  player.getComponent<Hylozoa::LocalTransform>().position = {100, 14.0f};
  player.addComponent<Hylozoa::Components::RigidBodyComponent>().type = b2_dynamicBody;
  player.addComponent<Hylozoa::Components::ColliderComponent>().enableContactEvents = true;

  auto &box = player.addComponent<Hylozoa::Components::BoxColliderComponent>();
  box.halfWidth = 1.0f;
  box.halfHeight = 1.0f;

  player.addComponent<Hylozoa::Components::Controllable>();
  player.addComponent<Hylozoa::Components::Module::Vision>(Hylozoa::Components::Module::Vision{
    .offset = {0.0f, 0.0f},
    .fov = 45.0f,
    .range = 200.0f,
    .direction = 90.0f,
    .rayCount = 10
  });
  std::cout << "Player Vision: "
            << player.getComponent<Hylozoa::Components::Module::Vision>().fov
            << " degrees." << std::endl;


  
  ground.getComponent<Hylozoa::LocalTransform>().position = {100.0f, 1000.0f};
  // ground.addComponent<Hylozoa::Components::ColliderComponent>().enableContactEvents = true;
  // ground.addComponent<Hylozoa::Components::Rendering::Renderable>(renderable);
  // ground.addComponent<Hylozoa::Components::Rendering::RenderableTexture>(
  //   Hylozoa::Components::Rendering::RenderableTexture{
  //         "assets/textures/Ground.png"
  //   }
  // );
  ground.addComponent<Hylozoa::Components::RigidBodyComponent>().type = b2_staticBody;
  
  // auto &groundbox = ground.addComponent<Hylozoa::Components::BoxColliderComponent>();
  // groundbox.halfWidth = 50.0f;
  // groundbox.halfHeight = 10.0f;
  // ground.addComponent<Hylozoa::Components::RigidBodyComponent>();

  // auto id = ground.getId();
  // std ::cout << "Ground entity ID: " << (uint32_t)id << std::endl;

  // b2WorldDef worldDef = b2DefaultWorldDef();
  // worldDef.gravity = {0.0f, -9.81f};
  // b2WorldId world = b2CreateWorld(&worldDef);
  
  // b2BodyDef bodyDef = b2DefaultBodyDef();
  // bodyDef.type = b2_staticBody;
  // bodyDef.position = b2Vec2(100.0f, 1000.0f);
  
  // b2BodyId body = b2CreateBody(world, &bodyDef);
  // b2Polygon polygon = b2MakeBox(50.0f, 10.0f); // va de 50 a 150 en x, de 990 a 1010 en y
  // b2ShapeDef shapeDef = b2DefaultShapeDef();
  // shapeDef.isSensor = false;
  // shapeDef.userData = (void*)ground.getId();
  


  // b2CreatePolygonShape(body, &shapeDef, &polygon);


  // auto visionReport = updateVision(
  //   player.getId(), 
  //   world, 
  //   {100.0f, 14.0f}, 
  //   90.0f, 
  //   45.0f, 
  //   1200.0f, 
  //   10
  // );
  // for (auto const& [entity, points] : visionReport) {
  //     std::cout << "Seen entity ID: " << (uint32_t)entity << " is seen at : " << std::endl;
  //     for (const auto& point : points) {
  //         std::cout << "  (" << point.x << ", " << point.y << ")" << std::endl;
  //     }
  // }


  // std::cout << "Player entity: " << player.getName(engine) << std::endl;
  // std::cout << "Camera entity: " << camera.getName(engine) << std::endl;

  // engine.runTick(90);
  // engine.runTick(-1);
  engine.run();
  return 0;
}
