/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Audio Core [header file]
*/

#pragma once

#include "Entity.hpp"
#include <SDL3_mixer/SDL_mixer.h>
#include <entt/entt.hpp>

#include "ResourceManager.hpp"

namespace Hylozoa {

constexpr unsigned int MAX_TRACKS =
    16; // Maximum number of played sound at the same time

/**
 * @class Audio
 * @brief Main audio system class for managing sound effects, music, and noise
 * events in the Hylozoa Engine.
 *
 * This class provides methods to play sound effects, music tracks, and noise
 * events associated with entities. It uses Audio Ressource Manager to load and
 * manage audio resources, and SDL_mixer for audio playback.
 */
class Audio {
  public:
    Audio(entt::registry &registry);
    ~Audio() { MIX_DestroyMixer(m_mixer); };

    void initialize();

    void playSound(const std::string &SoundName);
    void playMusic(const std::string &MusicName);
    void playNoise(const std::string &NoiseName, Entity &entity);

  private:
    entt::registry &m_registry;

    MIX_Mixer *m_mixer = nullptr;
    std::vector<MIX_Track *> m_tracks;

    bool m_disabled = false; // flag to indicate if the audio system is disabled
                             // (e.g., in headless mode)

  private:
    MIX_Track *getFreeTrack();
};

} // namespace Hylozoa
