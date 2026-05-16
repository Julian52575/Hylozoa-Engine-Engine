/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Resources [header]
*/

#ifndef RESOURCES_HPP_
#define RESOURCES_HPP_

#include <iostream>
#include <string>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include <SDL3_mixer/SDL_mixer.h>

#include "Hylozoa-Engine/SDL/SDL_Manager.hpp"

/**
 * @namespace Hylozoa::Resources
 * @brief Contains classes for managing various types of resources in the
 * Hylozoa Engine, such as textures, fonts, sounds, and music.
 *
 */
namespace Hylozoa::Resources {

/**
 * @class Texture
 * @brief Class for managing texture resources in the Hylozoa Engine.
 *
 * This class provides methods to load textures from files and retrieve their
 * properties. It uses SDL_Texture for texture management and is designed to
 * work with the SDL_Renderer provided by the SDL_Manager.
 */
class Texture {
  public:
    Texture() {
        m_renderer = Hylozoa::SDL::SDL_Manager::getInstance().getRenderer();
    }
    ~Texture() = default;

    Texture(const Texture &) = delete;
    Texture &operator=(const Texture &) = delete;

    SDL_Texture *get() const { return m_texture.get(); }
    SDL_FPoint getSize() const { return size; }

    bool loadFromFile(const std::string &filename);
    /**
     * @brief generate a template black and pink when loading a texture fails
     *
     * This function creates a simple checkerboard pattern texture (black and
     * pink) to be used as a fallback when loading a texture from a file fails.
     * The generated texture is 64x64 pixels in size, with alternating 32x32
     * pixel squares of magenta (pink) and black. This allows the engine to
     * visually indicate missing or failed textures in the rendered scene,
     * making it easier for developers to identify and debug texture loading
     * issues.
     *
     */
    void generateFallbackTexture();
    /**
     * @brief a loader function that can be used with the ResourceManager to
     * load texture resources. It takes a reference to a Texture object and a
     * filename, and returns a boolean indicating whether the loading was
     * successful. This function can be passed as a loader to the
     * ResourceManager when loading textures, allowing for easy integration of
     * texture loading into the resource management system.
     *
     * @param tex
     * @param filename
     * @return true - the loading was successful and the texture is ready to be
     * used.
     * @return false - the loading was unsuccessful (e.g., file not found,
     * unsupported format, etc.)
     */
    static bool loader(Texture &tex, const std::string &filename);

  private:
    std::shared_ptr<SDL_Renderer> m_renderer{nullptr};
    std::shared_ptr<SDL_Texture> m_texture{nullptr};
    SDL_FPoint size{0, 0};
};

class Font {
  public:
    bool loadFromFile(const std::string &filename);

  private:
};

/**
 * @class Sound
 * @brief Class for managing sound resources in the Hylozoa Engine.
 *
 * This class provides methods to load sound effects from files and retrieve the
 * underlying audio data. It uses MIX_Audio for sound management and is designed
 * to work with the MIX_Mixer provided by the Audio system.
 */
class Sound {
  public:
    Sound() = default;
    ~Sound() { MIX_DestroyAudio(m_audio); }

    MIX_Audio *get() const { return m_audio; }

    bool loadFromFile(const std::string &filename, MIX_Mixer &mixer);
    /**
     * @brief a loader function that can be used with the ResourceManager to
     * load sound resources. It takes a reference to a MIX_Mixer and returns a
     * lambda function that can be used to load Sound objects from files using
     * the provided mixer.
     *
     * @param mixer
     * @return auto
     */
    static auto loader(MIX_Mixer &mixer) {
        return [&mixer](Sound &sound, const std::string &filename) {
            return sound.loadFromFile(filename, mixer);
        };
    }

  private:
    MIX_Audio *m_audio = nullptr;
};

class Music {
  public:
    bool loadFromFile(const std::string &filename);

  private:
};

} // namespace Hylozoa::Resources

#endif /* !RESOURCES_HPP_ */