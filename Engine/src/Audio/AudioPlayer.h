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

struct ma_decoder;
struct ma_device;

namespace Engine::Audio {
    class AudioPlayer {
    public:
        AudioPlayer();
        ~AudioPlayer();
        AudioPlayer(const AudioPlayer&) = delete;
        AudioPlayer& operator=(const AudioPlayer&) = delete;

        bool   Load(const std::string& path);
        void   Play() const;
        void   Pause() const;
        void   SetLooping(bool loop) { m_Looping = loop; }
        void   SetVolume(float volume);
        float GetVolume() const;
        bool   IsLoaded() const { return m_Loaded; }
        double GetElapsedTime() const;
        void   GetLatestSamples(float* out, size_t count) const;

    private:
        void Unload();
        static void Callback(ma_device* device, void* out, const void* in, uint32_t frames);

        static constexpr uint32_t kSampleRate = 44100;
        static constexpr size_t   kRingSize   = 1 << 16;
        static constexpr size_t   kRingMask   = kRingSize - 1;

        ma_decoder* m_Decoder = nullptr;
        ma_device*  m_Device  = nullptr;
        bool        m_Loaded  = false;
        std::atomic<bool> m_Looping{false};

        std::array<float, kRingSize> m_Recent{};
        std::atomic<uint64_t> m_Written{0};
        std::atomic<uint64_t> m_Position{0};
        std::atomic<float> m_Volume{1.0};
    };
}


#endif //BLOCKGAME_AUDIOPLAYER_H
