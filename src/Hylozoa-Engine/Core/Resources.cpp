/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Resources [source]
*/

#include "Resources.hpp"

namespace Hylozoa::Resources
{
    bool Texture::loadFromFile(std::string_view filename)
    {
        std::string path = std::string(SDL_GetBasePath()) + "assets/" + std::string(filename);
        
        SDL_Texture* raw = IMG_LoadTexture(m_renderer.get(), path.c_str());
        
        if (!raw) {
            std::cerr << "IMG_LoadTexture failed (" << path << ")\n";
            return false;
        }

        float w, h;
        SDL_GetTextureSize(raw, &w, &h);
        size = {w, h};

        m_texture.reset(raw, SDL_DestroyTexture);
        return true;
    }




} // namespace Hylozoa::Resources
