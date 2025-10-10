/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Entity Class [source file]
*/

#include "Entity.hpp"
#include "Engine.hpp"

namespace Hylozoa {

Entity Entity::child_of(Entity parent, Engine &engine) {
  auto &registry = engine.get_registry();
  if (registry.valid(m_entity) &&
      registry.valid(parent.get_internal_entity())) {
    registry.emplace_or_replace<Parent>(m_entity,
                                        Parent{parent.get_internal_entity()});
  } else {
    std::cerr << "[Entity] Warning: Trying to set parent on invalid entity."
              << std::endl;
  }
  return *this;
}

const std::string &Entity::get_name(Engine &engine) const {
  auto &registry = engine.get_registry();
  if (registry.valid(this->m_entity) && registry.all_of<Name>(m_entity)) {
    return registry.get<Name>(m_entity).name;
  }
  throw std::runtime_error("[Entity] Error: Entity has no name or is invalid.");
}

} // namespace Hylozoa