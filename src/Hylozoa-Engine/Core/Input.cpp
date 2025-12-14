/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Input Core [source file]
*/

#include <iostream>

#include "Hylozoa-Engine/Core/Input.hpp"

#include "Hylozoa-Engine/Components/Context/Input.hpp"
#include "Hylozoa-Engine/Components/Context/Events.hpp"

namespace Hylozoa {

bool InputManager::isActionPressed(const std::string &action) const {
    return false; // Not implemented yet
}

void InputManager::bindAction(const std::string &action, const std::vector<SDL_Keycode> &keys) {
    m_actionBindings[action] = keys;
}

void InputManager::bindAction(const std::string &action, const SDL_Keycode key) {
    m_actionBindings[action].push_back(key);
}

bool InputManager::isKeyDown(std::string_view key) {
    const auto& inputState = m_registry.ctx().get<Components::HylozoaInternal::InputState>();

    return isKeyDown(resolveKey(key));
}

bool InputManager::isKeyHeld(std::string_view key) {
    const auto& inputState = m_registry.ctx().get<Components::HylozoaInternal::InputState>();

    return isKeyHeld(resolveKey(key));
}

bool InputManager::isKeyUp(std::string_view key) {
    const auto& inputState = m_registry.ctx().get<Components::HylozoaInternal::InputState>();

    return isKeyUp(resolveKey(key));
}


bool InputManager::isKeyDown(SDL_Scancode key) const {
    const auto& inputState = m_registry.ctx().get<Components::HylozoaInternal::InputState>();

    return inputState.keysPressed[key];
}

bool InputManager::isKeyHeld(SDL_Scancode key) const {
    const auto& inputState = m_registry.ctx().get<Components::HylozoaInternal::InputState>();

    return inputState.keysHeld[key];
}


bool InputManager::isKeyUp(SDL_Scancode key) const {
    const auto& inputState = m_registry.ctx().get<Components::HylozoaInternal::InputState>();

    return inputState.keysReleased[key];
}


void InputManager::pollEvents() const {
    beginFrame();
    auto &input = m_registry.ctx().get<Components::HylozoaInternal::InputState>();
    auto &mouseInput = m_registry.ctx().get<Components::HylozoaInternal::MouseState>();
    auto &events = m_registry.ctx().get<Components::HylozoaInternal::Events>();

    SDL_Event event;

    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_EVENT_QUIT:
        std::cout << "[Input] Quit event detected\n";
        events.shouldQuit = true;
        break;
  
      case SDL_EVENT_KEY_DOWN:
        input.keysPressed[event.key.scancode] = true;
        input.keysHeld[event.key.scancode] = true;
        break;
      case SDL_EVENT_KEY_UP:
        input.keysReleased[event.key.scancode] = true;
        input.keysHeld[event.key.scancode] = false;
        break;
  
      case SDL_EVENT_MOUSE_BUTTON_DOWN:
        if (event.button.button < mouseInput.buttonsPressed.size()) {
          mouseInput.buttonsPressed[event.button.button] = true;
          mouseInput.buttonsHeld[event.button.button] = true;
        }
        break;
      case SDL_EVENT_MOUSE_BUTTON_UP:
        if (event.button.button < mouseInput.buttonsReleased.size()) {
          mouseInput.buttonsReleased[event.button.button] = true;
          mouseInput.buttonsHeld[event.button.button] = false;
        }
        break;
      case SDL_EVENT_MOUSE_MOTION:
        mouseInput.x = event.motion.x;
        mouseInput.y = event.motion.y;
        break;
      case SDL_EVENT_MOUSE_WHEEL:
        mouseInput.wheelX = event.wheel.x;
        mouseInput.wheelY = event.wheel.y;
        break;
  
      default:
        break;
      }
    }
}


// Private methods

SDL_Scancode InputManager::resolveKey(std::string_view key) {
    auto it = m_keyCache.find(key.data());
    if (it != m_keyCache.end())
        return it->second;

    SDL_Scancode scanCode = SDL_GetScancodeFromName(key.data());
    m_keyCache.emplace(key, scanCode);
    return scanCode;
}

void InputManager::beginFrame() const {
    auto &input = m_registry.ctx().get<Components::HylozoaInternal::InputState>();
    auto &mouseInput = m_registry.ctx().get<Components::HylozoaInternal::MouseState>();

    input.keysPressed.fill(false);
    input.keysReleased.fill(false);
    mouseInput.buttonsPressed.fill(false);
    mouseInput.buttonsReleased.fill(false);
    mouseInput.wheelX = 0;
    mouseInput.wheelY = 0;
}


} // namespace Hylozoa