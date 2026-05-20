/*
** EPITECH PROJECT, 2026
** Hylozoa Engine
** File description:
** Tags Manager [source]
*/

#include "TagsManager.hpp"
#include <stdexcept>

namespace Hylozoa {


unsigned int TagsManager::registerTag(const std::string &name)
{
    if (m_tagsToId.contains(name)) {
        return m_tagsToId[name];
    }

    m_idToTag[m_nextTagId] = name;
    m_tagsToId[name] = m_nextTagId++;
    return m_tagsToId[name];
}

void TagsManager::unregisterTag(const std::string &name)
{
    if (!m_tagsToId.contains(name)) {
        return;
    }

    unsigned int tagId = m_tagsToId[name];
    m_tagsToId.erase(name);
    m_idToTag.erase(tagId);
}

void TagsManager::unregisterTag(unsigned int tagId)
{
    if (!m_idToTag.contains(tagId)) {
        return;
    }

    std::string name = m_idToTag[tagId];
    m_idToTag.erase(tagId);
    m_tagsToId.erase(name);
}

bool TagsManager::tagExist(const std::string &name) const
{
    return m_tagsToId.contains(name);
}

bool TagsManager::tagExist(unsigned int tagId) const
{
    return m_idToTag.contains(tagId);
}

std::vector<std::string> TagsManager::serializeTags(const std::vector<unsigned int> &tags) const
{
    std::vector<std::string> serializedTags;
    for (const unsigned int tagId : tags) {
        if (m_idToTag.contains(tagId)) {
            serializedTags.push_back(m_idToTag.at(tagId));
        }
    }
    return serializedTags;
}

std::vector<unsigned int> TagsManager::deserializeTags(const std::vector<std::string> &tagNames) const
{
    std::vector<unsigned int> deserializedTags;
    for (const std::string &name : tagNames) {
        if (m_tagsToId.contains(name)) {
            deserializedTags.push_back(m_tagsToId.at(name));
        }
    }
    return deserializedTags;
}

const std::unordered_map<std::string, unsigned int> &TagsManager::tags() const
{
    return m_tagsToId;
}

unsigned int TagsManager::getTagId(const std::string &name) const
{
    if (!tagExist(name)) {
        throw std::runtime_error("TagsManager::getTagId - Tag not found: " + name);
    }

    return m_tagsToId.at(name);
}

std::string TagsManager::getTagName(unsigned int tagId) const
{
    if (!tagExist(tagId)) {
        throw std::runtime_error("TagsManager::getTagName - Tag ID not found: " + std::to_string(tagId));
    }

    return m_idToTag.at(tagId);
}

} // namespace Hylozoa