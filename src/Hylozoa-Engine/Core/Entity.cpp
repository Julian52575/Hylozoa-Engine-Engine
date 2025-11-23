/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Entity Class [source file]
*/

#include "Entity.hpp"
#include "Engine.hpp"

namespace Hylozoa {

const Entity &Entity::childOf(Entity &parent) const {
  if (m_registry->valid(m_entity) && m_registry->valid(parent.getId())) {
    m_registry->emplace_or_replace<Components::Parent>(m_entity, Components::Parent{parent.getId()});
  } else {
    std::cerr << "[Entity] Warning: Trying to set parent on invalid entity. "
                 "Defaulting to no parent."
              << std::endl;
    m_registry->remove<Components::Parent>(m_entity);
  }
  return *this;
}

const Entity &Entity::childOf(entt::entity parent_entity) const {
  if (m_registry->valid(m_entity) && m_registry->valid(parent_entity)) {
    m_registry->emplace_or_replace<Components::Parent>(m_entity, Components::Parent{parent_entity});
  } else {
    std::cerr << "[Entity] Warning: Trying to set parent on invalid entity. "
                 "Defaulting to no parent."
              << std::endl;
    m_registry->remove<Components::Parent>(m_entity);
  }
  return *this;
}

const std::string &Entity::getName(Engine &engine) const {
  if (m_registry->valid(this->m_entity) && m_registry->all_of<Components::Name>(m_entity)) {
    return m_registry->get<Components::Name>(m_entity).name;
  }
  throw std::runtime_error("[Entity] Error: Entity has no name or is invalid.");
}

void Entity::destroy() {
  if (isValid()) {
    m_registry->destroy(m_entity);
  } else {
    std::cerr << "[Entity] Warning: Trying to destroy an invalid entity."
              << std::endl;
  }
}

} // namespace Hylozoa