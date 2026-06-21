/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Input Core [source file]
*/

#include <iostream>

#include "Input.hpp"

#include "Hylozoa-Engine/Components/Context/Events.hpp"
#include "Hylozoa-Engine/Components/Context/Input.hpp"

#include "Hylozoa-Engine/Core/Settings.hpp"

namespace Hylozoa {

bool Input::isActionPressed(const std::string &action) const {
    return false; // Not implemented yet
}

void Input::bindAction(const std::string &action,
                       const std::vector<SDL_Keycode> &keys) {
    m_actionBindings[action] = keys;
}

void Input::bindAction(const std::string &action, const SDL_Keycode key) {
    m_actionBindings[action].push_back(key);
}

bool Input::isKeyDown(std::string_view key) {
    auto tolook = resolveKey(key);
    if (tolook == SDL_SCANCODE_UNKNOWN) {
        if (Hylozoa::Settings::getInstance().getSettings().verbose) {
            std::cout << "[Input] Warning: Key '" << key
                      << "' could not be resolved to a valid scancode.\n";
        }
        return false;
    }
    return isKeyDown(tolook);
}

bool Input::isKeyHeld(std::string_view key) {
    auto tolook = resolveKey(key);
    if (tolook == SDL_SCANCODE_UNKNOWN) {
        if (Hylozoa::Settings::getInstance().getSettings().verbose) {
            std::cout << "[Input] Warning: Key '" << key
                      << "' could not be resolved to a valid scancode.\n";
        }
        return false;
    }
    return isKeyHeld(tolook);
}

bool Input::isKeyUp(std::string_view key) {
    auto tolook = resolveKey(key);
    if (tolook == SDL_SCANCODE_UNKNOWN) {
        if (Hylozoa::Settings::getInstance().getSettings().verbose) {
            std::cout << "[Input] Warning: Key '" << key
                      << "' could not be resolved to a valid scancode.\n";
        }
        return false;
    }
    return isKeyUp(tolook);
}

bool Input::isKeyDown(SDL_Scancode key) const {
    return m_inputState.keysPressed[key];
}

bool Input::isKeyHeld(SDL_Scancode key) const {
    return m_inputState.keysHeld[key];
}

bool Input::isKeyUp(SDL_Scancode key) const {
    return m_inputState.keysReleased[key];
}

void Input::pollEvents() const {
    beginFrame();
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_EVENT_QUIT:
            if (Hylozoa::Settings::getInstance().getSettings().verbose) {
                std::cout << "[Input] Quit event detected\n";
            }
            m_engineEvents.shouldQuit = true;
            break;

        case SDL_EVENT_KEY_DOWN:
            if (!event.key.repeat) {
                m_inputState.keysPressed[event.key.scancode] = true;
            }
            m_inputState.keysHeld[event.key.scancode] = true;
            break;
        case SDL_EVENT_KEY_UP:
            m_inputState.keysReleased[event.key.scancode] = true;
            m_inputState.keysHeld[event.key.scancode] = false;
            break;

        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            if (event.button.button < m_mouseState.buttonsPressed.size()) {
                m_mouseState.buttonsPressed[event.button.button] = true;
                m_mouseState.buttonsHeld[event.button.button] = true;
            }
            break;
        case SDL_EVENT_MOUSE_BUTTON_UP:
            if (event.button.button < m_mouseState.buttonsReleased.size()) {
                m_mouseState.buttonsReleased[event.button.button] = true;
                m_mouseState.buttonsHeld[event.button.button] = false;
            }
            break;
        case SDL_EVENT_MOUSE_MOTION:
            m_mouseState.x = event.motion.x;
            m_mouseState.y = event.motion.y;
            break;
        case SDL_EVENT_MOUSE_WHEEL:
            m_mouseState.wheelX = event.wheel.x;
            m_mouseState.wheelY = event.wheel.y;
            break;
        case SDL_EVENT_GAMEPAD_AXIS_MOTION:
            if (event.gdevice.which)

            default:
                break;
        }
    }
}

// Private methods

SDL_Scancode Input::resolveKey(std::string_view key) {
    auto it = m_keyCache.find(key.data());
    if (it != m_keyCache.end())
        return it->second;

    SDL_Scancode scanCode = SDL_GetScancodeFromName(key.data());
    m_keyCache.emplace(key, scanCode);
    return scanCode;
}

void Input::beginFrame() const {
    m_inputState.keysPressed.fill(false);
    m_inputState.keysReleased.fill(false);
    m_mouseState.buttonsPressed.fill(false);
    m_mouseState.buttonsReleased.fill(false);
    m_mouseState.wheelX = 0;
    m_mouseState.wheelY = 0;
}

} // namespace Hylozoa