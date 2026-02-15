/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Input Core [header file]
*/

#pragma once
#include <SDL3/SDL_keyboard.h>
#include <entt/entt.hpp>

namespace Hylozoa {

/*
 * @class Input
 * @brief Manages input handling for the engine.
 */
class Input {
  public:
    Input(entt::registry &registry) : m_registry(registry) {}
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

    // Low-level key checks, Users would generaly not use them directly but who
    // knows

    /*
     * @brief Check if a key is pressed down this frame.
     * @param key The SDL_Scancode of the key to check.
     */
    bool isKeyDown(SDL_Scancode key) const;
    /*
     * @brief Check if a key is still held down.
     * @param key The SDL_Scancode of the key to check.
     */
    bool isKeyHeld(SDL_Scancode key) const;
    /*
     * @brief Check if a key is released this frame.
     * @param key The SDL_Scancode of the key to check.
     */
    bool isKeyUp(SDL_Scancode key) const;

    // poll and process SDL events
    void pollEvents() const;
    // clear input states at the beginning of each frame
    void beginFrame() const;

  private:
    std::unordered_map<std::string, SDL_Scancode>
        m_keyCache; // for faster key lookup as resolving each frame can be
                    // costly
    std::unordered_map<std::string, std::vector<SDL_Keycode>>
        m_actionBindings; // later to set up the Frontend input mapping simillar
                          // to Godot
    entt::registry &m_registry;

    SDL_Scancode resolveKey(std::string_view key);
};

} // namespace Hylozoa