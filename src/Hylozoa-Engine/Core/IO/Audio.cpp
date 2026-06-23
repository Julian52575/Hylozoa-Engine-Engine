/*
** EPITECH PROJECT, 2025
** Hylozoa Engine
** File description:
** Audio Core [source file]
*/

#include "Audio.hpp"
#include "Hylozoa-Engine/Core/Resources/Resources.hpp"
#include "Hylozoa-Engine/Components/Components.hpp"
#include "Hylozoa-Engine/Core/Resources/Resources.hpp"
#include "Hylozoa-Engine/Core/Settings.hpp"

namespace Hylozoa {
Audio::Audio(entt::registry &registry) : m_registry(registry){};

Audio::~Audio() {
    for (auto *track : m_tracks) {
        MIX_DestroyTrack(track);
    }
    m_tracks.clear();

    if (m_mixer) {
        MIX_DestroyMixer(m_mixer);
    }
    MIX_Quit();
}

void Audio::playSound(const std::string &soundName) {
    if (m_disabled) {
        if (Hylozoa::Settings::getInstance().getSettings().verbose) {
            SDL_Log("[Audio] Audio system is disabled, cannot play sound: %s",
                    soundName.c_str());
        }
        return;
    }
    auto sound = m_registry.ctx().get<SoundManager>().load(
        Resources::Sound::loader(*m_mixer), soundName);

    MIX_Track *track = getFreeTrack();
    if (!track)
        return;

    MIX_SetTrackAudio(track, sound->get());
    MIX_PlayTrack(track, 0);
}

void Audio::playMusic(const std::string &musicName) { return; }

void Audio::playNoise(const std::string &noiseName, Entity &source) {
    if (m_disabled) {
        if (Hylozoa::Settings::getInstance().getSettings().verbose) {
            SDL_Log("[Audio] Audio system is disabled, cannot play noise: %s",
                    noiseName.c_str());
        }
        return;
    }
    auto sound = m_registry.ctx().get<SoundManager>().load(
        Resources::Sound::loader(*m_mixer), noiseName);

    MIX_Track *track = getFreeTrack();
    if (!track)
        return;

    auto view =
        m_registry.view<Components::MainListener, Components::WorldTransform>();
    auto &sourcePos = source.getComponent<Components::WorldTransform>();
    for (auto entity : view) {
        auto &transform = view.get<Components::WorldTransform>(entity);

        glm::vec2 relative = sourcePos.position - transform.position;

        MIX_Point3D point(relative.x, relative.y, 0);

        MIX_SetTrack3DPosition(track, &point);
        break;
    }

    MIX_SetTrackAudio(track, sound->get());
    MIX_PlayTrack(track, 0);
    m_registry.ctx()
        .get<Components::HylozoaInternal::EventsDispatcher>()
        .dispatcher.trigger<Components::HylozoaInternal::OnNoiseEvent>(
            {source.getHandle(), noiseName, sourcePos.position});
    return;
}

MIX_Track *Audio::getFreeTrack() {
    for (auto *track : m_tracks) {
        if (!MIX_TrackPlaying(track))
            return track;
    }
    return nullptr;
}

} // namespace Hylozoa