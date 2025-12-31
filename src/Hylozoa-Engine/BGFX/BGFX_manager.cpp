#include "BGFX_manager.hpp"

#include <bimg/bimg.h>

#include <bx/allocator.h>
#include <bx/macros.h>
#include <bx/file.h>
#include <bx/error.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Hylozoa::BGFX {
    // bgfx::TextureHandle bgfx_manager::loadTexture(const char* filepath){
        // bx::DefaultAllocator allocator;
        // bx::FileReader reader;

        // if (!bx::open(&reader, filepath)) {
        //     SDL_Log("Couldn't open texture file: %s", filepath);
        //     exit(84);
        //     return BGFX_INVALID_HANDLE;
        // }

        // uint32_t size = (uint32_t)bx::getSize(&reader);
        // void* fileData = bx::alloc(&allocator, size);

        // bx::Error err;
        // bx::read(&reader, fileData, size, &err);
        // bx::close(&reader);

        // if (!err.isOk()) {
        //     bx::free(&allocator, fileData);
        //     SDL_Log("Failed to read texture file");
        //     exit(84);
        //     return BGFX_INVALID_HANDLE;
        // }

        // bimg::ImageContainer image;
        // if (!bimg::imageParse(image, fileData, size, &err)) {
        //     bx::free(&allocator, fileData);
        //     SDL_Log("bimg::imageParse failed: %s", err.getMessage().getCPtr());
        //     exit(84);
        //     return BGFX_INVALID_HANDLE;
        // }

        // // IMPORTANT : on peut maintenant libérer fileData
        // // bx::free(&allocator, fileData);

        // const bgfx::Memory* mem = bgfx::copy(
        //     image.m_data,
        //     image.m_size
        // );

        // bgfx::TextureHandle texture = bgfx::createTexture2D(
        //     (uint16_t)image.m_width,
        //     (uint16_t)image.m_height,
        //     image.m_numMips > 1,
        //     image.m_numLayers,
        //     bgfx::TextureFormat::Enum(image.m_format),
        //     BGFX_TEXTURE_NONE,
        //     mem
        // );

        // // Libérer l’image APRÈS le copy
        // bimg::imageFree(&image);
        // bx::free(&allocator, fileData);

        // if (!bgfx::isValid(texture)) {
        //     SDL_Log("createTexture2D failed");
        //     exit(84);
        // }
        // SDL_Log("Texture loaded: %s", filepath);
        // exit(0);

        // return texture;
    // }

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