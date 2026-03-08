/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Audio Core [source file]
*/

#include "Audio.hpp"
#include "Resources.hpp"

#include "Hylozoa-Engine/Components/Components.hpp"

namespace Hylozoa
{
Audio::Audio(entt::registry &registry) : m_registry(registry)
{
    if (!MIX_Init()) {
        SDL_Log("MIX_Init failed: %s", SDL_GetError());
    } else {
        SDL_Log("SDL_mixer is ready!");
    }

    SDL_AudioSpec hint{};
    hint.freq = 44100;
    hint.format = SDL_AUDIO_F32;
    hint.channels = 2;

    m_mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &hint);

    for (int i = 0; i < MAX_TRACKS; ++i)
    {
        MIX_Track* track = MIX_CreateTrack(m_mixer);
        m_tracks.push_back(track);
    }
}

void Audio::playSound(const std::string &soundName)
{
    auto sound = m_registry.ctx().get<SoundManager>().load(Resources::Sound::loader(*m_mixer), soundName);

    MIX_Track* track = getFreeTrack();
    if (!track)
        return;

    MIX_SetTrackAudio(track, sound->get());
    MIX_PlayTrack(track, 0);
}

void Audio::playMusic(const std::string& musicName)
{
    return;
}

void Audio::playNoise(const std::string& noiseName, Entity& source)
{
    auto sound = m_registry.ctx().get<SoundManager>().load(Resources::Sound::loader(*m_mixer), noiseName);

    MIX_Track* track = getFreeTrack();
    if (!track)
        return;

    auto view = m_registry.view<Components::MainListener, Components::WorldTransform>();
    auto& sourcePos = source.getComponent<Components::WorldTransform>();
    for (auto entity : view)
    {
        auto& transform = view.get<Components::WorldTransform>(entity);

        glm::vec2 relative = sourcePos.position - transform.position;

        MIX_Point3D point(relative.x, relative.y, 0);

        MIX_SetTrack3DPosition(track, &point);
        break;
    }
    
    MIX_SetTrackAudio(track, sound->get());
    MIX_PlayTrack(track, 0);
    m_registry.ctx().get<Components::HylozoaInternal::EventsDispatcher>().dispatcher.trigger<Components::HylozoaInternal::OnNoiseEvent>({source.getHandle(), noiseName, sourcePos.position, 1.0f});
    return;
}


MIX_Track* Audio::getFreeTrack()
{
    for (auto* track : m_tracks)
    {
        if (!MIX_TrackPlaying(track))
            return track;
    }
    return nullptr;
}

}