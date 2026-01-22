/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Entity Class [header]
*/

#ifndef ENTITY_HPP_
#define ENTITY_HPP_

#include "Hylozoa-Engine/Components/Transform/Transform.hpp"
#include <entt/entt.hpp>

namespace Hylozoa {
class Engine;

/*
 * @class Entity
 * @brief A class representing an entity in the Hylozoa Engine.
 *
 * This class provides methods for managing components and relationships
 * of an entity within an entt registry.
 */
class Entity {
  public:
    /*
     * @brief Constructs an Entity with the given entt entity and registry.
     */
    Entity(entt::entity entity, entt::registry &registry)
        : m_entity(entity), m_registry(&registry) {}
    ~Entity() = default;

    // --- Component Management ---

    /*
    * @brief Adds a component of type T to the entity.

  * @tparam T The type of the component to add.
  * @returns A reference to the added component.
  * @throws std::runtime_error if the component already exists on the entity.
  * @warning Does not support adding tag components (empty struct). Use addTag<T>() instead.
  */
  template <typename T, typename... Args> T &addComponent(Args &&...args) {
    if (m_registry->all_of<T>(m_entity))
      throw std::runtime_error("Component already exists on entity.");
    return m_registry->emplace<T>(m_entity, std::forward<Args>(args)...);
  }


  /*
  * @brief Adds a tag component of type T to the entity.
  * @tparam T The type of the tag component to add.
  * @throws std::runtime_error if the tag already exists on the entity.
  */
  template <typename T> void addTag() {
    if (m_registry->all_of<T>(m_entity))
      throw std::runtime_error("Tag already exists on entity.");
    m_registry->emplace<T>(m_entity);
  }

  /*
   * @brief Removes the component of type T from the entity.
   * @tparam T The type of the component to remove.
   * @throws std::runtime_error if the component does not exist on the entity.
   */
  template <typename T> void removeComponent() {
    if (!m_registry)
      throw std::runtime_error("Registry is null.");
    if (!m_registry->all_of<T>(m_entity))
      throw std::runtime_error("Component does not exist on entity.");
    m_registry->remove<T>(m_entity);
  }

  /*
   * @brief Retrieves a reference to the component of type T attached to the
   * entity.
   * @tparam T The type of the component to retrieve.
   * @returns A reference to the requested component.
   */
  template <typename T> T &getComponent() {
    if (!m_registry)
      throw std::runtime_error("Registry is null.");
    if (!m_registry->all_of<T>(m_entity))
      throw std::runtime_error("Component does not exist on entity.");
    return m_registry->get<T>(m_entity);
  }

  /*
   * @brief Checks if the entity has a component of type T.
   * @tparam T The type of the component to check.
   * @returns true if the component exists, false otherwise.
   */
  template <typename T> bool hasComponent() const {
    if (!m_registry)
      throw std::runtime_error("Registry is null.");
    return m_registry->all_of<T>(m_entity);
  }

    // --- Relationship Management ---

  /*
   * @brief Sets the parent of the entity to the specified parent entity.
   * @param parent The parent entity (Hyloza Entity).
   */
  const Entity &childOf(Entity &parent) const;
  /*
   * @brief Sets the parent of the entity to the specified parent entity.
   * @param parentEntity The parent entity (EnTT Entity).
   */
  const Entity &childOf(entt::entity parentEntity) const;

    // --- Utility Functions ---

    /*
     * @brief Destroys the entity and removes it from the registry.
     */
    void destroy();
    /*
     * @brief Checks if the entity is valid (exists in the registry).
     */
    const bool isValid() const { return m_registry->valid(m_entity); }
    /*
     * @brief Retrieves the name of the entity.
     */
    const std::string &getName(Engine &engine) const;
    /*
     * @brief Retrieves the EnTT entity ID of the entity.
     * @returns The EnTT entity ID.
     * @warning THis should only be used internally!
     */
    entt::entity getId() const { return m_entity; }

  private:
    /*
     * @var m_entity
     *      The EnTT entity ID.
     * @var m_registry
     *      Pointer to the EnTT registry managing the entity.
     */
    entt::entity m_entity{entt::null};
    entt::registry *m_registry{nullptr};
};

} // namespace Hylozoa

#endif /* !ENTITY_HPP_ */
