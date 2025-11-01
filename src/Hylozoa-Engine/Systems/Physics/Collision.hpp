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

namespace Hylozoa {
    class CollisionSystem : public System {
        public:
            const std::string& getName() const override { return this->_name; }

            void onStart() override {
                b2WorldDef worldDef = b2DefaultWorldDef();
                worldDef.gravity = b2Vec2(0.0f, -9.81f);
                m_world = b2CreateWorld(&worldDef);
                std::cout << "[" << this->_name << "] Start\n";
            }

            void onUpdate(float dt) override {
                if (this->_registry) {
                    b2World_Step(m_world, dt, 4);
                    syncTransforms();
                    processEvents();
                }
            }

            void createBodies();
            void createColliders();

            void syncTransforms();
            void processEvents();

            void onEnd() override {
                b2DestroyWorld(m_world);
            }
        private:
            std::string _name = "CollisionSystem";
            b2WorldId m_world{b2_nullWorldId};
    };
} // namespace Hylozoa

#endif /* !COLLISION_HPP_ */