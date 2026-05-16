/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Physics related System [header]
*/

#ifndef PHYSICS_SYSTEM_HPP_
#define PHYSICS_SYSTEM_HPP_

#include "Hylozoa-Engine/Components/Context/Events.hpp"
#include "Hylozoa-Engine/Core/Settings.hpp"
#include "Hylozoa-Engine/Systems/Manager/Systems.hpp"
#include <box2d/box2d.h>
#include <iostream>

namespace Hylozoa {
namespace Systems {
class PhysicsSystem : public System {
  public:
    PhysicsSystem(entt::registry &registry) : System(registry)
    {
      auto &dispatcher =
      this->_registry.ctx()
          .get<Components::HylozoaInternal::EventsDispatcher>();
  
      dispatcher.dispatcher.sink<Components::HylozoaInternal::OnEntityDestroyed>()
          .connect<&PhysicsSystem::onEntityDestroyed>(this);
    }
    const std::string &getName() const override { return this->_name; }

    void onStart() override {
        b2WorldDef worldDef = b2DefaultWorldDef();
        worldDef.gravity = b2Vec2{0.0f, 0.981f}; // Gravity downwards
        m_world = b2CreateWorld(&worldDef);

        if (Hylozoa::Settings::getInstance().getSettings().verbose) {
            std::cout << "[" << this->_name << "] Start\n";
        }
    }

    void onUpdate(float dt) override {
        syncECStoBox2D();
        createBodies();
        createColliders();
        b2World_Step(m_world, dt, 4);
        processEvents();
        syncBox2DtoECS();
    }

    void onEntityDestroyed(const Components::HylozoaInternal::OnEntityDestroyed &event);
    void onSceneLoaded(const uint64_t sceneId) override;
    void onSceneUnloaded(const uint64_t sceneId) override;

    void onEnd() override { b2DestroyWorld(m_world); }

  private:
    std::string _name = "CollisionSystem";
    b2WorldId m_world{b2_nullWorldId};

  private:
    void createBodies();
    void createColliders();
    void syncECStoBox2D();
    void syncBox2DtoECS();
    void processEvents();
};
} // namespace Systems
} // namespace Hylozoa

#endif /* !PHYSICS_SYSTEM_HPP_ */