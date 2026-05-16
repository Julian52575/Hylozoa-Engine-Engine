/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Entity Class [header]
*/

#ifndef ENTITY_HPP_
#define ENTITY_HPP_

#include "Hylozoa-Engine/Components/Components.hpp"
#include <entt/entt.hpp>
#include <typeinfo>

namespace Hylozoa {
class Engine;

/**
 * @class Entity
 * @brief A class representing an entity in the Hylozoa Engine.
 *
 * This class provides methods for managing components and relationships
 * of an entity within an entt registry.
 */
class Entity {
  public:
    Entity(entt::entity entity, entt::registry &registry)
        : m_entity(entity), m_registry(registry) {}
    ~Entity() = default;

    // --- Component Management ---

    /**
     * @brief Adds a component of type T to the entity.
     * @tparam T The type of the component to add.
     * @returns A reference to the added component.
     * @throws std::runtime_error if the component already exists on the entity.
     * @warning Does not support adding tag components (empty struct). Use
     * addTagComponentComponent<T>() instead.
     */
    template <typename T, typename... Args> T &addComponent(Args &&...args) {
        if (m_registry.all_of<T>(m_entity))
            throw std::runtime_error(
                std::string("Entity::addComponent - Component already exists "
                            "on entity. (") +
                typeid(T).name() + std::string(")"));
        return m_registry.emplace<T>(m_entity, std::forward<Args>(args)...);
    }

    /**
     * @brief Adds a tag component of type T to the entity.
     * @tparam T The type of the tag component to add.
     * @throws std::runtime_error if the tag already exists on the entity.
     */
    template <typename T> void addTagComponent() {
        if (m_registry.all_of<T>(m_entity))
            throw std::runtime_error(
                std::string("Entity::addTagComponent - Tag already exists on "
                            "entity. (") +
                typeid(T).name() + std::string(")"));
        m_registry.emplace_or_replace<T>(m_entity);
    }

    /**
     * @brief Removes the component of type T from the entity.
     * @tparam T The type of the component to remove.
     * @throws std::runtime_error if the component does not exist on the entity.
     */
    template <typename T> void removeComponent() {
        if (!m_registry.all_of<T>(m_entity))
            throw std::runtime_error(
                std::string("Entity::removeComponent - Component does not "
                            "exist on entity. (") +
                typeid(T).name() + std::string(")"));
        m_registry.remove<T>(m_entity);
    }

    /**
     * @brief Retrieves a reference to the component of type T attached to the
     * entity.
     * @tparam T The type of the component to retrieve.
     * @returns A reference to the requested component.
     */
    template <typename T> T &getComponent() {
        if (!m_registry.all_of<T>(m_entity))
            throw std::runtime_error(
                std::string("Entity::getComponent - Component does not exist "
                            "on entity. (") +
                typeid(T).name() + std::string(")"));
        ;
        return m_registry.get<T>(m_entity);
    }

    /**
     * @brief Checks if the entity has a component of type T.
     * @tparam T The type of the component to check.
     * @returns true if the component exists, false otherwise.
     */
    template <typename T> bool hasComponent() const {
        return m_registry.all_of<T>(m_entity);
    }

    // --- Relationship Management ---

    /**
     * @brief Sets the parent of the entity to the specified parent entity.
     * @param parent The parent entity (Hyloza Entity).
     */
    const Entity &childOf(Entity &parent) const;
    /**
     * @brief Sets the parent of the entity to the specified parent entity.
     * @param parentEntity The parent entity (EnTT Entity).
     */
    const Entity &childOf(entt::entity parentEntity) const;
    /**
     * @brief Sets the parent of the entity to the specified parent UUID.
     * @param parentUUID The UUID of the parent entity.
     */
    const Entity &childOf(UUID parentUUID) const;

    // --- Utility Functions ---

    /**
     * @brief Destroys the entity and removes it from the registry.
     */
    void destroy();
    /**
     * @brief Checks if the entity is valid (exists in the registry).
     */
    const bool isValid() const { return m_registry.valid(m_entity); }
    /**
     * @brief Retrieves the name of the entity.
     */
    const std::string &getName() const;
    /**
     * @brief Retrieves the EnTT entity ID of the entity.
     * @returns The EnTT entity ID.
     * @warning This should only be used internally!
     */
    entt::entity getHandle() const { return m_entity; }

    /**
     * @brief retrieves the Entity instance from an EnTT entity handle and
     * registry.
     *
     * @param entity the entt entity handle to convert to an Entity instance
     * @param registry the entt registry that contains the entity
     * @return Entity
     */
    static Entity fromHandle(entt::entity entity, entt::registry &registry) {
        return Entity(entity, registry);
    }

  private:
    entt::entity m_entity{entt::null};
    entt::registry &m_registry;
};

} // namespace Hylozoa

#endif /* !ENTITY_HPP_ */
