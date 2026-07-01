/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Input Core [header file]
*/

#pragma once
#include "Hylozoa-Engine/Components/Components.hpp"
#include <SDL3/SDL_keyboard.h>
#include <entt/entt.hpp>

namespace Hylozoa {

/**
 * @class Input
 * @brief Manages input handling for the engine.
 */
class Input {
  public:
    Input(entt::registry &registry) : m_registry(registry),
    m_inputState(registry.ctx().get<Components::HylozoaInternal::InputState>()),
    m_mouseState(registry.ctx().get<Components::HylozoaInternal::MouseState>()),
    m_engineEvents(registry.ctx().get<Components::HylozoaInternal::EngineEvents>()) {}
    ~Input() = default;

    // please dont use those for now, they are here for future implementation of
    // action mapping. i'm working on it okay ?

    bool isActionPressed(const std::string &action) const;
    void bindAction(const std::string &action,
                    const std::vector<SDL_Keycode> &keys);
    void bindAction(const std::string &action, const SDL_Keycode key);

    // High-level key checks

    /*
     * @brief Check if a key is pressed down this frame.
     * @param key The key to check (e.g., "A", "Space", "LeftCtrl").
     */
    bool isKeyDown(std::string_view key);
    /*
     * @brief Check if a key is still held down.
     * @param key The key to check (e.g., "A", "Space", "LeftCtrl").
     */
    bool isKeyHeld(std::string_view key);
    /*
     * @brief Check if a key is released this frame.
     * @param key The key to check (e.g., "A", "Space", "LeftCtrl").
     */
    bool isKeyUp(std::string_view key);

    /**
     * @brief Check if a mouse button is pressed down this frame.
     * @param button The mouse button to check (e.g., "Left", "Right", "Middle").
     */
    bool isMouseButtonDown(std::string_view button);
    /**
     * @brief Check if a mouse button is still held down.
     * @param button The mouse button to check (e.g., "Left", "Right", "Middle").
     */
    bool isMouseButtonHeld(std::string_view button);
    /**
     * @brief Check if a mouse button is released this frame.
     * @param button The mouse button to check (e.g., "Left", "Right", "Middle").
     */
    bool isMouseButtonUp(std::string_view button);

    // Low-level key checks, Users would generaly not use them directly but who
    // knows

    /**
     * @brief Check if a key is pressed down this frame.
     * @param key The SDL_Scancode of the key to check.
     */
    bool isKeyDown(SDL_Scancode key) const;
    /**
     * @brief Check if a key is still held down.
     * @param key The SDL_Scancode of the key to check.
     */
    bool isKeyHeld(SDL_Scancode key) const;
    /**
     * @brief Check if a key is released this frame.
     * @param key The SDL_Scancode of the key to check.
     */
    bool isKeyUp(SDL_Scancode key) const;

    /**
     * @brief Check if a mouse button is pressed down this frame.
     * @param button the SDL_MouseButtonFlags of the mouse button to check.
     */
    bool isMouseButtonDown(SDL_MouseButtonFlags button) const;
    /**
     * @brief Check if a mouse button is still held down.
     * @param button the SDL_MouseButtonFlags of the mouse button to check.
     */
    bool isMouseButtonHeld(SDL_MouseButtonFlags button) const;
    /**
     * @brief Check if a mouse button is released this frame.
     * @param button the SDL_MouseButtonFlags of the mouse button to check.
     */
    bool isMouseButtonUp(SDL_MouseButtonFlags button) const;

    // poll and process SDL events
    void pollEvents() const;
    // clear input states at the beginning of each frame
    void beginFrame() const;

  private:
    std::unordered_map<std::string, SDL_Scancode>
        m_keyCache; // for faster key lookup as resolving each frame can be
                    // costly
    std::unordered_map<std::string, SDL_MouseButtonFlags>
        m_mouseButtonCache; // for faster mouse button lookup as resolving each
                            // frame can be costly
    std::unordered_map<std::string, std::vector<SDL_Keycode>>
        m_actionBindings; // later to set up the Frontend input mapping simillar
                          // to Godot
    Components::HylozoaInternal::InputState& m_inputState;
    Components::HylozoaInternal::MouseState& m_mouseState;
    Components::HylozoaInternal::EngineEvents& m_engineEvents;
    entt::registry &m_registry;

    SDL_Scancode resolveKey(std::string_view key);
    SDL_MouseButtonFlags resolveMouseButton(std::string_view button);
};

} // namespace Hylozoa