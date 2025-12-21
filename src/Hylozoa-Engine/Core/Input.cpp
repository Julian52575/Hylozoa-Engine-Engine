/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Input Core [source file]
*/

#include <iostream>

#include "Hylozoa-Engine/Core/Input.hpp"

#include "Hylozoa-Engine/Components/Context/Events.hpp"
#include "Hylozoa-Engine/Components/Context/Input.hpp"

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
  const auto &inputState =
      m_registry.ctx().get<Components::HylozoaInternal::InputState>();

  return isKeyDown(resolveKey(key));
}

bool Input::isKeyHeld(std::string_view key) {
  const auto &inputState =
      m_registry.ctx().get<Components::HylozoaInternal::InputState>();

  return isKeyHeld(resolveKey(key));
}

bool Input::isKeyUp(std::string_view key) {
  const auto &inputState =
      m_registry.ctx().get<Components::HylozoaInternal::InputState>();

  return isKeyUp(resolveKey(key));
}

bool Input::isKeyDown(SDL_Scancode key) const {
  const auto &inputState =
      m_registry.ctx().get<Components::HylozoaInternal::InputState>();

  return inputState.keysPressed[key];
}

bool Input::isKeyHeld(SDL_Scancode key) const {
  const auto &inputState =
      m_registry.ctx().get<Components::HylozoaInternal::InputState>();

  return inputState.keysHeld[key];
}

bool Input::isKeyUp(SDL_Scancode key) const {
  const auto &inputState =
      m_registry.ctx().get<Components::HylozoaInternal::InputState>();

  return inputState.keysReleased[key];
}

void Input::pollEvents() const {
  beginFrame();
  auto &input = m_registry.ctx().get<Components::HylozoaInternal::InputState>();
  auto &mouseInput =
      m_registry.ctx().get<Components::HylozoaInternal::MouseState>();
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
  auto &input = m_registry.ctx().get<Components::HylozoaInternal::InputState>();
  auto &mouseInput =
      m_registry.ctx().get<Components::HylozoaInternal::MouseState>();

  input.keysPressed.fill(false);
  input.keysReleased.fill(false);
  mouseInput.buttonsPressed.fill(false);
  mouseInput.buttonsReleased.fill(false);
  mouseInput.wheelX = 0;
  mouseInput.wheelY = 0;
}

} // namespace Hylozoa