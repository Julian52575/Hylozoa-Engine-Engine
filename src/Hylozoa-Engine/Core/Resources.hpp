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
            
            bool loadFromFile(std::string_view filename);
        private:
            std::shared_ptr<SDL_Renderer> m_renderer{nullptr};
            std::shared_ptr<SDL_Texture> m_texture{nullptr};
            SDL_FPoint size{0, 0};
    };

    class Font {
    };

    class Audio {
        public:
        private:
        
    };


} // namespace Hylozoa

#endif /* !RESOURCES_HPP_ */