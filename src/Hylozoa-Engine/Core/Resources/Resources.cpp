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
        std::cerr << "Texture::loadFromFile() - IMG_LoadTexture failed (" << filename << ")\n";
        const int texSize = 64;
        const int checkSize = 32;
        
        // Create a blank texture (RGBA8888 is standard for SDL3)
        raw = SDL_CreateTexture(m_renderer.get(), SDL_PIXELFORMAT_RGBA8888, 
                                 SDL_TEXTUREACCESS_STATIC, texSize, texSize);
        
        if (!raw) {
            std::cerr << "Texture::loadFromFile() (ERROR!)- SDL_CreateTexture failed for placeholder texture: " << SDL_GetError() << "\n";
            return false;
        }

        uint32_t pixels[texSize * texSize];
        for (int y = 0; y < texSize; y++) {
            for (int x = 0; x < texSize; x++) {
                bool isPink = ((x / checkSize) + (y / checkSize)) % 2 == 0;
                pixels[y * texSize + x] = isPink ? 0xFF00FFFF : 0xFF000000;
            }
        }

        SDL_UpdateTexture(raw, NULL, pixels, texSize * sizeof(uint32_t));
        // Set blend mode so the magenta is solid
        SDL_SetTextureBlendMode(raw, SDL_BLENDMODE_BLEND);
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
