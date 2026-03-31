/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Resources [source]
*/

#include "Resources.hpp"

namespace Hylozoa::Resources {
bool Texture::loadFromFile(const std::string &filename) {
    std::string path = std::string(SDL_GetBasePath()) + "assets/" + filename;

    SDL_Texture *raw = IMG_LoadTexture(m_renderer.get(), path.c_str());

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

bool Texture::loader(Texture &tex, const std::string &filename) {
    return tex.loadFromFile(filename);
}

bool Font::loadFromFile(const std::string &filename) { return false; }

bool Sound::loadFromFile(const std::string &filename, MIX_Mixer &mixer) {
    std::string path = std::string(SDL_GetBasePath()) + "assets/" + filename;

    m_audio = MIX_LoadAudio(&mixer, path.c_str(), true);

    if (!m_audio) {
        std::cerr << "MIX_LoadAudio failed (" << path << ")\n";
        return false;
    }

    return true;
}

bool Music::loadFromFile(const std::string &filename) { return false; }

} // namespace Hylozoa::Resources
