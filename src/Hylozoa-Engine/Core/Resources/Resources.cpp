/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Resources [source]
*/

#include "Resources.hpp"

namespace Hylozoa::Resources {
bool Texture::loadFromFile(const std::string &filename) {
    SDL_Texture *raw = IMG_LoadTexture(m_renderer.get(), filename.c_str());

    if (!raw) {
        std::cerr << "Texture::loadFromFile() - IMG_LoadTexture failed ("
                  << filename << ")\n";
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
    m_audio = MIX_LoadAudio(&mixer, filename.c_str(), true);

    if (!m_audio) {
        std::cerr << "MIX_LoadAudio failed (" << filename << ")\n";
        return false;
    }

    return true;
}

bool Music::loadFromFile(const std::string &filename) { return false; }

} // namespace Hylozoa::Resources
