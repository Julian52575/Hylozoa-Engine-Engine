/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Entity Class [header]
*/

#ifndef ENTITY_HPP_
#define ENTITY_HPP_

#include <flecs.h>

namespace Hylozoa {

    class Entity {
        public:
            Entity(flecs::entity e) : m_entity(e) {}
            Entity(flecs::world& world) : m_entity(world.entity()) {}
            ~Entity() = default;

            template<typename Component>
            void removeComponent() {
                m_entity.remove<Component>();
            }

            template<typename Component>
            Component& getComponent() {
                return m_entity.get<Component>().value();
            }

            template<typename Component>
            bool hasComponent() {
                return m_entity.has<Component>();
            }

        private:
            flecs::entity m_entity;
    };

}

#endif /* !ENTITY_HPP_ */
