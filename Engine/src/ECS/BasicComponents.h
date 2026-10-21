//
// Created by Kreiseljustus on 10/14/2026.
//

#ifndef BLOCKGAME_BASICCOMPONENTS_H
#define BLOCKGAME_BASICCOMPONENTS_H

#include <string>
#include <glm/glm.hpp>

#include "Audio/AudioPlayer.h"
#include "Rendering/RenderData.h"

namespace Engine::Components {

    using namespace Engine::Rendering;

    struct TagComponent {
        std::string name;
    };

    struct MeshRendererComponent {
        MeshHandle mesh;
        Material material;
    };

    struct AudioSourceComponent {
        std::unique_ptr<Audio::AudioPlayer> player = std::make_unique<Audio::AudioPlayer>();

        [[nodiscard]] bool   Load(const std::string& path) const { return player->Load(path); }
        void   Play() const                        { player->Play(); }
        void   Pause() const                       { player->Pause(); }
        void   SetLooping(const bool loop) const         { player->SetLooping(loop); }
        void   SetVolume(const float v) const            { player->SetVolume(v); }
        [[nodiscard]] double GetTime() const               { return player->GetElapsedTime(); }
        void   GetLatestSamples(float* out, const size_t n) const { player->GetLatestSamples(out, n); }
    };
}

#endif //BLOCKGAME_BASICCOMPONENTS_H
