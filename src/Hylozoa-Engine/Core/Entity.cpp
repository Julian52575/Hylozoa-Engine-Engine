/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Entity Class [source file]
*/

#include "Entity.hpp"
#include "Engine.hpp"

namespace Hylozoa {

const Entity &Entity::child_of(Entity &parent) const {
  if (m_registry->valid(m_entity) && m_registry->valid(parent.get_id())) {
    m_registry->emplace_or_replace<Parent>(m_entity, Parent{parent.get_id()});
  } else {
    std::cerr << "[Entity] Warning: Trying to set parent on invalid entity. "
                 "Defaulting to no parent."
              << std::endl;
    m_registry->remove<Parent>(m_entity);
  }
  return *this;
}

const Entity &Entity::child_of(entt::entity parent_entity) const {
  if (m_registry->valid(m_entity) && m_registry->valid(parent_entity)) {
    m_registry->emplace_or_replace<Parent>(m_entity, Parent{parent_entity});
  } else {
    std::cerr << "[Entity] Warning: Trying to set parent on invalid entity. "
                 "Defaulting to no parent."
              << std::endl;
    m_registry->remove<Parent>(m_entity);
  }
  return *this;
}

const std::string &Entity::get_name(Engine &engine) const {
  if (m_registry->valid(this->m_entity) && m_registry->all_of<Name>(m_entity)) {
    return m_registry->get<Name>(m_entity).name;
  }
  throw std::runtime_error("[Entity] Error: Entity has no name or is invalid.");
}

void Entity::destroy() {
  if (is_valid()) {
    m_registry->destroy(m_entity);
  } else {
    std::cerr << "[Entity] Warning: Trying to destroy an invalid entity."
              << std::endl;
  }
}

} // namespace Hylozoa