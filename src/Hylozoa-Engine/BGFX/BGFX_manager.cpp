#include "BGFX_manager.hpp"

#include <bimg/bimg.h>

#include <bx/allocator.h>
#include <bx/macros.h>
#include <bx/file.h>
#include <bx/error.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Hylozoa::BGFX {
    bgfx::TextureHandle bgfx_manager::loadTexture(const char* filepath,float *outWidth, float *outHeight){
        int width, height, channels;
        
        // 1. Charger les pixels bruts (RGBA = 4 channels)
        // STB décompresse le PNG en mémoire
        unsigned char* data = stbi_load(filepath, &width, &height, &channels, 4);

        if (!data) {
            SDL_Log("STB failed to load %s", filepath);
            return BGFX_INVALID_HANDLE;
        }

        // 2. Allouer la mémoire pour BGFX
        // bgfx::makeRef ou copy prend des pixels BRUTS, pas le fichier PNG
        const bgfx::Memory* mem = bgfx::copy(data, width * height * 4);

        // 3. On peut libérer les données STB immédiatement après le copy
        stbi_image_free(data);

        // 4. Créer la texture
        bgfx::TextureHandle texture = bgfx::createTexture2D(
            static_cast<uint16_t>(width),
            static_cast<uint16_t>(height),
            false, // pas de mips
            1,     // 1 couche
            bgfx::TextureFormat::RGBA8, // On force RGBA8 car STB a sorti 4 channels
            BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP | BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT,
            mem
        );

        if (!bgfx::isValid(texture)) {
            SDL_Log("Failed to create GPU texture for %s", filepath);
            exit(84);
            return BGFX_INVALID_HANDLE;
        }

        if (outWidth) {
            *outWidth = width;
        }
        if (outHeight) {
            *outHeight = height;
        }

        return texture;
    }

}