/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Resources [header]
*/

#ifndef RESOURCES_HPP_
#define RESOURCES_HPP_

#include <string>
#include <iostream>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include <SDL3_mixer/SDL_mixer.h>

#include "Hylozoa-Engine/SDL/SDL_Manager.hpp"

namespace Hylozoa::Resources {

    class Texture {
        public:
            Texture() { m_renderer = Hylozoa::SDL::SDL_Manager::getInstance().getRenderer(); }
            ~Texture() = default;

            Texture(const Texture&) = delete;
            Texture& operator=(const Texture&) = delete;

            SDL_Texture* get() const { return m_texture.get(); }
            SDL_FPoint getSize() const { return size; }
            
            bool loadFromFile(const std::string& filename);
            static bool loader(Texture& tex, const std::string& filename);
        private:
            std::shared_ptr<SDL_Renderer> m_renderer{nullptr};
            std::shared_ptr<SDL_Texture> m_texture{nullptr};
            SDL_FPoint size{0, 0};
    };

    class Font {
        public:
            bool loadFromFile(const std::string& filename);
        private:
    };

    class Sound {
        public:
            Sound() = default;
            ~Sound() { MIX_DestroyAudio(m_audio); }

            MIX_Audio* get() const { return m_audio; }

            bool loadFromFile(const std::string& filename, MIX_Mixer &mixer);
            static auto loader(MIX_Mixer& mixer)
            {
                return [&mixer](Sound& s, const std::string& filename)
                {
                    return s.loadFromFile(filename, mixer);
                };
            }
        

        private:
            MIX_Audio* m_audio = nullptr;
    };

    class Music {
        public:
            bool loadFromFile(const std::string& filename);
        private:
    };


} // namespace Hylozoa

#endif /* !RESOURCES_HPP_ */