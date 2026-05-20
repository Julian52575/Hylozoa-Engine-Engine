/*
** EPITECH PROJECT, 2026
** Hylozoa Engine
** File description:
** Tags Manager [header]
*/

#ifndef TAGS_MANAGER_HPP_
#define TAGS_MANAGER_HPP_

#include <vector>
#include <unordered_map>
#include <string>
#include <cstdint>

namespace Hylozoa {

class TagsManager {
public:
    TagsManager(const TagsManager &) = delete;
    TagsManager &operator=(const TagsManager &) = delete;

    static TagsManager &instance() {
        static TagsManager instance;
        return instance;
    }

    unsigned int registerTag(const std::string &name);
    void unregisterTag(const std::string &name);
    void unregisterTag(const unsigned int tagId);

    bool tagExist(const std::string &name) const;
    bool tagExist(const unsigned int tagId) const;

    std::vector<std::string> serializeTags(const std::vector<unsigned int> &tags) const;
    std::vector<unsigned int> deserializeTags(const std::vector<std::string> &tagNames) const;

    const std::unordered_map<std::string, unsigned int> &tags() const;
    unsigned int getTagId(const std::string &name) const;
    std::string getTagName(unsigned int tagId) const;
private:
    TagsManager() = default;
    std::unordered_map<std::string, unsigned int> m_tagsToId;
    std::unordered_map<unsigned int, std::string> m_idToTag;
    unsigned int m_nextTagId = 0;
};

}


#endif /* !TAGS_MANAGER_HPP_ */