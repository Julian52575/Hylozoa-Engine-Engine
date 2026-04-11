/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** ResourceManager [header]
*/

#ifndef RESOURCEMANAGER_HPP_
#define RESOURCEMANAGER_HPP_

#include <string>
#include <unordered_map>

#include "Resources.hpp"
#include "Hylozoa-Engine/Core/Settings.hpp"

namespace Hylozoa {

/**
 * @brief A Templated Ressources Manager class to keep track of loaded
 * Ressources.
 *
 * @tparam Resource the type of ressources to be managed
 */
template <typename Resource> class ResourcesManager {
  public:
    /**
     * @brief load a Ressource from a file
     *
     * this function return a shared pointer to the Ressource, if a ressource
     * has already been loaded then it return the corresponding pointer to the
     * ressource.
     *
     * @param filename path to the file to be loaded
     * @return A std::shared_ptr<Resource> of the loaded ressource
     */
    template <typename Loader>
    std::shared_ptr<Resource> load(Loader loader, const std::string &filename);

    /**
     * @brief unload a Ressource from a file
     *
     * @param filename path to the ressource to be unloaded
     */
    void unload(const std::string &filename);

    /**
     * @brief reset all loaded Ressources
     */
    void reset() { m_resources.clear(); }

  private:
    std::unordered_map<std::string, std::shared_ptr<Resource>> m_resources;
};

template <typename Resource>
template <typename Loader>
std::shared_ptr<Resource>
ResourcesManager<Resource>::load(Loader loader, const std::string &filename) {
    if (m_resources.contains(filename)) {
        return m_resources[filename];
    }

    const char* base = SDL_GetBasePath();
    std::string fullPath = std::string(base ? base : "") + "Assets/" + filename;
    if (Hylozoa::Settings::getInstance().getSettings().verbose) {
        std::cout << "[ResourceManager] Loading resource: " << fullPath << std::endl;
    }

    auto resource = std::make_shared<Resource>();

    if (!loader(*resource, fullPath)) {
        std::cerr << "ResourceManager::load() - Failed to load resource: " << filename << std::endl;
        
        if (m_resources.contains("__FALLBACK__")) {
            m_resources[filename] = m_resources["__FALLBACK__"];
        }else {
            m_resources["__FALLBACK__"] = resource;
            m_resources[filename] = resource;
        }
        return m_resources[filename];
    }

    m_resources[filename] = resource;
    return m_resources[filename];
}

template <typename Resource>
void ResourcesManager<Resource>::unload(const std::string &filename) {
    auto it = m_resources.find(filename);

    if (it != m_resources.end()) {
        m_resources.erase(it);
    } else {
        throw std::runtime_error("ResourceManager::unload() - Trying to "
                                 "unload, unloaded resource: " +
                                 filename);
    }
}

using TextureManager = ResourcesManager<Resources::Texture>;
using SoundManager = ResourcesManager<Resources::Sound>;
using FontManager = ResourcesManager<Resources::Font>;

} // namespace Hylozoa

#endif /* !RESOURCEMANAGER_HPP_ */