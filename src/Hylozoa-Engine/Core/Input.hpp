/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Input Core [header file]
*/

#pragma once
#include <entt/entt.hpp>
#include <SDL3/SDL_keyboard.h>

namespace Hylozoa {

    /*
    * @class InputManager
    * @brief Manages input handling for the engine.
    */
    class InputManager {
    public:
        InputManager(entt::registry &registry) : m_registry(registry) {}
        ~InputManager() = default;

        // please dont use those for now, they are here for future implementation of action mapping. i'm working on it okay ?

        bool isActionPressed(const std::string &action) const;
        void bindAction(const std::string &action, const std::vector<SDL_Keycode> &keys);
        void bindAction(const std::string &action, const SDL_Keycode key);


        bool isKeyDown(std::string_view key);
        bool isKeyHeld(std::string_view key);
        bool isKeyUp(std::string_view key);


        // Low-level key checks, Users would generaly not use them directly but who knows

        bool isKeyDown(SDL_Scancode key) const;
        bool isKeyHeld(SDL_Scancode key) const;
        bool isKeyUp(SDL_Scancode key) const;

        void pollEvents() const;
        void beginFrame() const;


    private:
        std::unordered_map<std::string, SDL_Scancode> m_keyCache; // for faster key lookup as resolving each frame can be costly
        std::unordered_map<std::string, std::vector<SDL_Keycode>> m_actionBindings; // later to set up the Frontend input mapping simillar to Godot
        entt::registry &m_registry;

        SDL_Scancode resolveKey(std::string_view key);
    };

} // namespace Hylozoa