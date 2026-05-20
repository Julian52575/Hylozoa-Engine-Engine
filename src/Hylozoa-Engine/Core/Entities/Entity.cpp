/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Entity Class [source file]
*/

#include <iostream>

#include "Entity.hpp"
#include "Hylozoa-Engine/Core/Tags/TagsManager.hpp"

namespace Hylozoa {

const Entity &Entity::childOf(Entity &parent) const {
    if (m_registry.valid(m_entity) && m_registry.valid(parent.getHandle())) {
        m_registry.emplace_or_replace<Components::HylozoaInternal::Parent>(
            m_entity, Components::HylozoaInternal::Parent{parent.getHandle()});
    } else {
        std::cerr << "Entity::childOf(Entity) - Warning: Trying to set parent "
                     "on invalid entity. "
                     "Defaulting to no parent."
                  << std::endl;
        m_registry.remove<Components::HylozoaInternal::Parent>(m_entity);
    }
    return *this;
}

const Entity &Entity::childOf(entt::entity parentEntity) const {
    if (m_registry.valid(m_entity) && m_registry.valid(parentEntity)) {
        m_registry.emplace_or_replace<Components::HylozoaInternal::Parent>(
            m_entity, Components::HylozoaInternal::Parent{parentEntity});
    } else {
        std::cerr << "Entity::childOf(entt::entity) - Warning: Trying to set "
                     "parent on invalid entity. "
                     "Defaulting to no parent."
                  << std::endl;
        m_registry.remove<Components::HylozoaInternal::Parent>(m_entity);
    }
    return *this;
}

const Entity &Entity::childOf(UUID parentUUID) const {
    auto view = m_registry.view<Components::HylozoaInternal::Id>();
    for (auto entity : view) {
        auto &uuidComp = view.get<Components::HylozoaInternal::Id>(entity);
        if (uuidComp.id == parentUUID) {
            return childOf(entity);
        }
    }
    std::cerr << "Entity::childOf(UUID) - Warning: Trying to set parent on "
                 "invalid entity. "
                 "Defaulting to no parent."
              << std::endl;
    m_registry.remove<Components::HylozoaInternal::Parent>(m_entity);
    return *this;
}

const std::string &Entity::getName() const {
    if (m_registry.valid(m_entity) &&
        m_registry.all_of<Components::Name>(m_entity)) {
        return m_registry.get<Components::Name>(m_entity).name;
    }
    throw std::runtime_error(
        "Entity::getName - Entity has no name or is invalid.");
}

void Entity::destroy() {
    if (!isValid()) {
        std::cerr
            << "Entity::destroy - Warning: Trying to destroy an invalid entity."
            << std::endl;
        return;
    }


    auto* parentComp = m_registry.try_get<Components::HylozoaInternal::Parent>(m_entity);
    if (parentComp) {
        auto parent = parentComp->entity;

        if (m_registry.valid(parent)) {
            if (auto* childrenComp = m_registry.try_get<Components::HylozoaInternal::Children>(parent)) {
                childrenComp->children.erase(m_entity);
            }
        }
    }

    auto* childrenComp = m_registry.try_get<Components::HylozoaInternal::Children>(m_entity);
    if (childrenComp) {
        auto children = childrenComp->children;

        for (auto child : children) {
            if (m_registry.valid(child)) {
                Entity::fromHandle(child, m_registry).destroy();
            }
        }
    }

    auto &eventsDispatcher = m_registry.ctx()
                             .get<Components::HylozoaInternal::EventsDispatcher>();
    eventsDispatcher.dispatcher.trigger(Components::HylozoaInternal::OnEntityDestroyed{m_entity});
    m_registry.destroy(m_entity);
}

// --- Tag Management ---

bool Entity::hasTag(const std::string &tagName) const {
    if (!isValid() || !hasComponent<Components::Tags>()) return false;

    auto& TagsManager = TagsManager::instance();
    if (!TagsManager.tagExist(tagName)) {
        std::cerr << "Entity::hasTag - Warning: Trying to check for non-existent tag '" << tagName << "'.\n";
        return false;
    }

    unsigned int tagId = TagsManager.getTagId(tagName);
    auto &tagsComponent = getComponent<Components::Tags>();

    return std::find(tagsComponent.tags.begin(), tagsComponent.tags.end(), tagId) != tagsComponent.tags.end();
}

bool Entity::hasTag(const unsigned int tag) const {
    if (!isValid() || !hasComponent<Components::Tags>()) return false;

    auto& TagsManager = TagsManager::instance();
    if (!TagsManager.tagExist(tag)) {
        std::cerr << "Entity::hasTag - Warning: Trying to check for non-existent tag ID " << tag << ".\n";
        return false;
    }

    auto &tagsComponent = getComponent<Components::Tags>();

    return std::find(tagsComponent.tags.begin(), tagsComponent.tags.end(), tag) != tagsComponent.tags.end();
}

bool Entity::addTag(const std::string &tagName) const {
    if (!isValid() || !hasComponent<Components::Tags>()) {
        return false;
    }

    auto& TagsManager = TagsManager::instance();
    if (!TagsManager.tagExist(tagName)) {
        std::cerr << "Entity::addTag - Warning: Trying to add unregistered tag '" << tagName << "'.\n";
        return false;
    }

    unsigned int tagId = TagsManager.getTagId(tagName);
    auto &tagsComponent = getComponent<Components::Tags>();

    if (std::find(tagsComponent.tags.begin(), tagsComponent.tags.end(), tagId) != tagsComponent.tags.end()) {
        std::cerr << "Entity::addTag - Warning: Entity already has tag '" << tagName << "'.\n";
        return false;
    }

    tagsComponent.tags.push_back(tagId);
    return true;
}

bool Entity::addTag(unsigned int tag) const {
    if (!isValid() || !hasComponent<Components::Tags>()) {
        return false;
    }

    auto& TagsManager = TagsManager::instance();
    if (!TagsManager.tagExist(tag)) {
        std::cerr << "Entity::addTag - Warning: Trying to add unregistered tag ID " << tag << ".\n";
        return false;
    }

    auto &tagsComponent = getComponent<Components::Tags>();

    if (std::find(tagsComponent.tags.begin(), tagsComponent.tags.end(), tag) != tagsComponent.tags.end()) {
        std::cerr << "Entity::addTag - Warning: Entity already has tag ID " << tag << ".\n";
        return false;
    }

    tagsComponent.tags.push_back(tag);
    return true;
}

bool Entity::removeTag(const std::string &tagName) const {
    if (!isValid() || !hasComponent<Components::Tags>()) {
        return false;
    }

    auto& TagsManager = TagsManager::instance();
    if (!TagsManager.tagExist(tagName)) {
        std::cerr << "Entity::removeTag - Warning: Trying to remove unregistered tag '" << tagName << "'.\n";
        return false;
    }

    unsigned int tagId = TagsManager.getTagId(tagName);
    auto &tagsComponent = getComponent<Components::Tags>();

    auto it = std::find(tagsComponent.tags.begin(), tagsComponent.tags.end(), tagId);
    if (it == tagsComponent.tags.end()) {
        std::cerr << "Entity::removeTag - Warning: Entity does not have tag '" << tagName << "'.\n";
        return false;
    }

    tagsComponent.tags.erase(it);
    return true;
}

bool Entity::removeTag(unsigned int tag) const {
    if (!isValid() || !hasComponent<Components::Tags>()) {
        return false;
    }

    auto& TagsManager = TagsManager::instance();
    if (!TagsManager.tagExist(tag)) {
        std::cerr << "Entity::removeTag - Warning: Trying to remove unregistered tag ID " << tag << ".\n";
        return false;
    }

    auto &tagsComponent = getComponent<Components::Tags>();

    auto it = std::find(tagsComponent.tags.begin(), tagsComponent.tags.end(), tag);
    if (it == tagsComponent.tags.end()) {
        std::cerr << "Entity::removeTag - Warning: Entity does not have tag ID " << tag << ".\n";
        return false;
    }

    tagsComponent.tags.erase(it);
    return true;
}

} // namespace Hylozoa