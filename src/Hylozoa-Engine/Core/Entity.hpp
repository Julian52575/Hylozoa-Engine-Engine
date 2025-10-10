/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Entity Class [header]
*/

#ifndef ENTITY_HPP_
#define ENTITY_HPP_

#include "Hylozoa-Engine/Components/Transform.hpp"
#include <entt/entt.hpp>

namespace Hylozoa {
class Engine;

class Entity {
public:
  Entity() = default;
  Entity(entt::entity entity) : m_entity(entity) {}
  ~Entity() = default;

  Entity child_of(Entity parent, Engine &engine);

  const std::string &get_name(Engine &engine) const;

  entt::entity get_internal_entity() const { return m_entity; }

private:
  entt::entity m_entity{entt::null};
};

} // namespace Hylozoa

#endif /* !ENTITY_HPP_ */
