/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Audio Core [header file]
*/

#pragma once

#include <SDL3_mixer/SDL_mixer.h>
#include <entt/entt.hpp>
#include "Entity.hpp"

#include "ResourceManager.hpp"

namespace Hylozoa
{

constexpr int MAX_TRACKS = 16; // Maximum number of played sound at the same time

class Audio
{
    public:
       Audio(entt::registry &registry);
       ~Audio() { MIX_DestroyMixer(m_mixer); };

       void playSound(const std::string &SoundName);
       void playMusic(const std::string &MusicName);
       void playNoise(const std::string &NoiseName, Entity &entity);
    private:
        entt::registry &m_registry;

        MIX_Mixer* m_mixer = nullptr;
        std::vector<MIX_Track*> m_tracks;

    private:
        MIX_Track* getFreeTrack();
};

} // namespace Hylozoa
