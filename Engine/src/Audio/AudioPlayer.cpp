#include <miniaudio.h>
#include "AudioPlayer.h"
#include <algorithm>

namespace Engine::Audio {
    AudioPlayer::AudioPlayer() {
        m_Decoder = new ma_decoder;
        m_Device = new ma_device;
    }
    AudioPlayer::~AudioPlayer() {
        Unload();
        delete m_Device;
        delete m_Decoder;
    }

    void AudioPlayer::Unload() {
        if (!m_Loaded) return;
        ma_device_uninit(m_Device);
        ma_decoder_uninit(m_Decoder);
        m_Loaded = false;
        m_Written = 0;
        m_Position = 0;
    }

    bool AudioPlayer::Load(const std::string& path) {
        Unload();

        ma_decoder_config dc = ma_decoder_config_init(ma_format_f32, 2, kSampleRate);
        if (ma_decoder_init_file(path.c_str(), &dc, m_Decoder) != MA_SUCCESS) return false;

        ma_device_config cfg = ma_device_config_init(ma_device_type_playback);
        cfg.playback.format   = ma_format_f32;
        cfg.playback.channels = 2;
        cfg.sampleRate        = kSampleRate;
        cfg.dataCallback      = Callback;
        cfg.pUserData         = this;

        if (ma_device_init(nullptr, &cfg, m_Device) != MA_SUCCESS) {
            ma_decoder_uninit(m_Decoder);
            return false;
        }
        m_Loaded = true;
        return true;
    }

    void AudioPlayer::Play() const {
        if (m_Loaded) ma_device_start(m_Device);
    }
    void AudioPlayer::Pause() const {
        if (m_Loaded) ma_device_stop(m_Device);
    }
    void AudioPlayer::SetVolume(const float volume) {
        if (m_Loaded) ma_device_set_master_volume(m_Device, std::clamp(volume, 0.0f,1.0f)); m_Volume = volume;
    }

    float AudioPlayer::GetVolume() const {
        return m_Volume.load();
    }

    void AudioPlayer::Callback(ma_device* dev, void* out, const void*, uint32_t frames) {
        auto* self = static_cast<AudioPlayer*>(dev->pUserData);
        auto* f = static_cast<float*>(out);

        ma_uint64 got = 0;
        ma_decoder_read_pcm_frames(self->m_Decoder, f, frames, &got);
        uint64_t pos = self->m_Position.load() + got;

        if (got < frames && self->m_Looping) {
            ma_decoder_seek_to_pcm_frame(self->m_Decoder, 0);
            ma_uint64 more = 0;
            ma_decoder_read_pcm_frames(self->m_Decoder, f + got * 2, frames - got, &more);
            got += more;
            pos = more;
        }

        for (ma_uint64 i = got * 2; i < static_cast<ma_uint64>(frames) * 2; i++) f[i] = 0.0f;

        const uint64_t w = self->m_Written.load();
        for (ma_uint64 i = 0; i < got; i++)
            self->m_Recent[(w + i) & kRingMask] = 0.5f * (f[i * 2] + f[i * 2 + 1]);
        self->m_Written.store(w + got);
        self->m_Position.store(pos);
    }

    double AudioPlayer::GetElapsedTime() const {
        return static_cast<double>(m_Position.load()) / kSampleRate;
    }

    void AudioPlayer::GetLatestSamples(float* out, const size_t count) const {
        const uint64_t w = m_Written.load();
        if (w < count) { for (size_t i = 0; i < count; i++) out[i] = 0.0f; return; }
        for (size_t i = 0; i < count; i++) out[i] = m_Recent[(w - count + i) & kRingMask];
    }
}