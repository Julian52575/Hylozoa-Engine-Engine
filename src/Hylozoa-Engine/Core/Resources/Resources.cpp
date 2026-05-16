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
                  << filename << "). Fallback Texture generated\n";
        generateFallbackTexture();
        return false;
    }

    float w, h;
    SDL_GetTextureSize(raw, &w, &h);
    size = {w, h};

    m_texture.reset(raw, SDL_DestroyTexture);
    return true;
}

void Texture::generateFallbackTexture() {
    const int texSize = 64;
    const int checkSize = 32;

    SDL_Texture *raw =
        SDL_CreateTexture(m_renderer.get(), SDL_PIXELFORMAT_RGBA8888,
                          SDL_TEXTUREACCESS_STATIC, texSize, texSize);
    if (raw) {
        uint32_t pixels[texSize * texSize];
        for (int i = 0; i < texSize * texSize; i++) {
            int x = i % texSize;
            int y = i / texSize;
            bool isPink = ((x / checkSize) + (y / checkSize)) % 2 == 0;
            pixels[i] = isPink ? 0xFF00FFFF : 0xFF000000;
        }

        SDL_UpdateTexture(raw, NULL, pixels, texSize * sizeof(uint32_t));
        float w, h;
        SDL_GetTextureSize(raw, &w, &h);
        this->size = {w, h};
        this->m_texture.reset(raw, SDL_DestroyTexture);
    }
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
