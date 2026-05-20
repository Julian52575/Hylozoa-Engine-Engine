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
    template <typename T> T &getComponent() const {
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

    /**
     * @brief checks if the entity is pending destruction (i.e., marked for destruction but not yet destroyed).
     * 
     * @return true has the PendingDestruction tag, meaning it is pending destruction
     * @return false does not have the PendingDestruction tag, meaning it is not pending destruction
     */
    bool isPendingDestruction() {
        if (m_registry.all_of<Components::HylozoaInternal::PendingDestruction>(m_entity))
            return true;
        return false;
    }

    /// --- Tag Management ---

    /**
     * @brief check if the entity has a specific tag.
     * 
     * @param tagName name of the tag to check.
     * @return true if the entity has the tag
     * @return false if the entity does not have the tag or if the tag does not exist in the TagsManager.
     */
    bool hasTag(const std::string &tagName) const;

    /**
     * @brief check if the entity has a specific tag.
     * 
     * @param tagId id of the tag to check
     * @return true if the entity has the tag
     * @return false if the entity does not have the tag or if the tag does not exist in the TagsManager.
     */
    bool hasTag(unsigned int tagId) const;

    /**
     * @brief add a specific tag to the entity.
     * 
     * This function adds the specified tag to the entity's Tags component, allowing for user organization.
     * The tag need to be registered in the TagsManager beforehand, and the entity will need to have a Tags component (which can be added using addComponent<Components::Tags>()).
     * 
     * @warning to not be confused with addTagComponent, which adds a tag COMPONENTS (empty struct) to the entity, while this adds a TAG (unsigned int ID) to the entity's Tags component.
     * @param tagName name of the tag to add
     * @return true tag successfully added to the entity
     * @return false if an error occurs (e.g., tag does not exist in the TagsManager, entity does not have a Tags component, etc.)
     */
    bool addTag(const std::string &tagName) const;

    /**
     * @brief add a specific tag to the entity.
     * 
     * This function adds the specified tag to the entity's Tags component, allowing for user organization.
     * The tag need to be registered in the TagsManager beforehand, and the entity will need to have a Tags component (which can be added using addComponent<Components::Tags>()).
     * 
     * @param tagId id of the tag to add
     * @return true tag successfully added to the entity
     * @return false if an error occurs (e.g., tag does not exist in the TagsManager, entity does not have a Tags component, etc.)
     * @warning to not be confused with addTagComponent, which adds a tag COMPONENTS (empty struct) to the entity, while this adds a TAG (unsigned int ID) to the entity's Tags component.
     */
    bool addTag(unsigned int tagId) const;


    /**
     * @brief remove a specific tag from the entity.
     * 
     * This function removes the specified tag from the entity's Tags component, allowing for user organization.
     * 
     * @param tagName name of the tag to remove
     * @return true tag successfully removed from the entity
     * @return false if an error occurs (e.g., tag does not exist in the TagsManager, entity does not have a Tags component, etc.)
     */
    bool removeTag(const std::string &tagName) const;

    /**
     * @brief remove a specific tag from the entity.
     * 
     * @param tagId id of the tag to remove
     * @return true tag successfully removed from the entity
     * @return false if an error occurs (e.g., tag does not exist in the TagsManager, entity does not have a Tags component, etc.)
     */
    bool removeTag(unsigned int tagId) const;

  private:
    entt::entity m_entity{entt::null};
    entt::registry &m_registry;
};

} // namespace Hylozoa

#endif /* !ENTITY_HPP_ */
