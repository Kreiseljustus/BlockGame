//
// Created by Kreiseljustus on 10/20/2026.
//

//Probably completely temp file, will be rewritten into ECS

#ifndef BLOCKGAME_AUDIOPLAYER_H
#define BLOCKGAME_AUDIOPLAYER_H

#include <string>
#include <atomic>
#include <cstdint>
#include <array>

#include "../headerLibs/miniaudio.h"

namespace Engine::Audio {
    class AudioPlayer {
    public:
        AudioPlayer();
        ~AudioPlayer();
        bool Load(const std::string& path);
        void Play() const;
        void Pause() const;
        void SetVolume(float volume);
        float GetVolume() {return m_Volume;}
        [[nodiscard]] double GetElapsedTime() const;
        void GetLatestSamples(float* out, size_t count) const;
    private:
        static void Callback(ma_device*, void* out, const void*, uint32_t frames);
        ma_decoder* m_Decoder;
        ma_device* m_Device;
        //Amount of Samples in Buffer for GetLatestSamples
        static constexpr size_t kSize = 1 << 16;
        std::array<float, kSize> m_Recent{};
        std::atomic<uint64_t> m_Written{0};
        float m_Volume{.5f};
    };
}


#endif //BLOCKGAME_AUDIOPLAYER_H
