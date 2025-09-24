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
            Entity() = default;
            explicit Entity(flecs::entity e) : m_entity(e) {}
            Entity(flecs::world& world) : m_entity(world.entity()) {}

            template <typename T, typename... Args>
            Entity& addComponent(Args&&... args) {
                m_entity.set<T>(std::forward<Args>(args)...);
                return *this;
            }

            template <typename T>
            Entity& addComponent(const T& value) {
                m_entity.set<T>(value);
                return *this;
            }

            template <typename T>
            const T& get() const{
                return m_entity.get<T>();
            }

            template <typename T>
            const T* tryGet() {
                return m_entity.try_get<T>();
            }

            template <typename T>
            Entity& remove() {
                m_entity.remove<T>();
                return *this;
            }

            void destruct() {
                m_entity.destruct();
            }

            Entity target(flecs::entity_t relationship) {
                return Entity { m_entity.target(relationship) };
            }

            
            const flecs::string_view name() const {
                return m_entity.name();
            }



            const Entity& childOf(const Entity& parent) {
                m_entity.child_of(parent.m_entity);
                return *this;
            }


            /**
             * @brief Get the Internal Entity object
             * 
             * This function is intended for internal use only.
             * 
             * @return flecs::entity& 
             */
            flecs::entity& getInternalEntity() {
                return m_entity;
            }

        private:
            flecs::entity m_entity;
    };

}

#endif /* !ENTITY_HPP_ */
