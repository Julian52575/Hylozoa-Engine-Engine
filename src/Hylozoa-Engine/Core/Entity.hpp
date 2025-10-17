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
  Entity(entt::entity entity, entt::registry& registry) : m_entity(entity),  m_registry(&registry) {}
  ~Entity() = default;

  const Entity& child_of(Entity parent) const;
  const std::string &get_name(Engine &engine) const;

  // --- Component Management ---

  template<typename T, typename... Args>
  T& add_component(Args&&... args) {
    return m_registry->emplace<T>(m_entity, std::forward<Args>(args)...);
  }

  template<typename T>
  void remove_component() {
    m_registry->remove<T>(m_entity);
  }


  entt::entity get_id() const { return m_entity; }

private:
  entt::entity m_entity{entt::null};
  entt::registry* m_registry{nullptr};
};

} // namespace Hylozoa

#endif /* !ENTITY_HPP_ */
