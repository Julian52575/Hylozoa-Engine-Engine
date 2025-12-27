/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Collision related System [header]
*/

#ifndef COLLISION_HPP_
#define COLLISION_HPP_

#include "Hylozoa-Engine/Systems/Manager/Systems.hpp"
#include <box2d/box2d.h>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <cmath>

namespace Hylozoa {
  typedef struct RayCastCallback {
        b2BodyId hitBody;
        void* userData;
        b2Vec2 hitPoint;
  } RayCastCallback;
class CollisionSystem : public System {
  public:
    const std::string &getName() const override { return this->_name; }

    void onStart() override {
      b2WorldDef worldDef = b2DefaultWorldDef();
      worldDef.gravity = (b2Vec2){0.0f, -9.81f};
      m_world = b2CreateWorld(&worldDef);

      std::cout << "[" << this->_name << "] Start\n";
    }

    void onUpdate(float dt) override {
      if (this->_registry) {
        createBodies();
        createColliders();
        createVisionShapes();
        b2World_Step(m_world, dt, 4);
        processEvents();
        processRaycasts();
        syncTransforms();
      }
    }

    void createBodies();
    void createColliders();

    void syncTransforms();
    void processEvents();


    // partie raycasting
    void createVisionShapes();
    void processRaycasts();
    static float rayCast(b2ShapeId shapeId, b2Vec2 point, b2Vec2 normal, float fraction, void* context);
    std::unordered_map<entt::entity, std::vector<b2Vec2>> updateVision(entt::entity observer, b2Vec2 pos, float dirDegrees, float fovDegrees, float range, int rayCount);


    void onEnd() override { b2DestroyWorld(m_world); }

  private:
    std::string _name = "CollisionSystem";
    b2WorldId m_world{b2_nullWorldId};
  };
} // namespace Hylozoa

#endif /* !COLLISION_HPP_ */