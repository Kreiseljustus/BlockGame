//
// Created by Kreiseljustus on 10/20/2026.
//

#include "AudioPlayer.h"

#include <algorithm>

using namespace Engine::Audio;

AudioPlayer::AudioPlayer()  { m_Decoder = new ma_decoder; m_Device = new ma_device; }
AudioPlayer::~AudioPlayer() { ma_device_uninit(m_Device); ma_decoder_uninit(m_Decoder); delete m_Device; delete m_Decoder; }

bool AudioPlayer::Load(const std::string& path) {
    ma_decoder_config dc = ma_decoder_config_init(ma_format_f32, 2, 44100);
    if (ma_decoder_init_file(path.c_str(), &dc, m_Decoder) != MA_SUCCESS) return false;

    ma_device_config cfg = ma_device_config_init(ma_device_type_playback);
    cfg.playback.format   = ma_format_f32;
    cfg.playback.channels = 2;
    cfg.sampleRate        = 44100;
    cfg.dataCallback      = Callback;
    cfg.pUserData         = this;
    return ma_device_init(nullptr, &cfg, m_Device) == MA_SUCCESS;
}

void AudioPlayer::Play() const { ma_device_start(m_Device); }
void AudioPlayer::Pause() const { ma_device_stop(m_Device); }

void AudioPlayer::SetVolume(const float volume) {
    m_Volume = std::ranges::clamp(volume, 0.0f, 1.0f);
}

void AudioPlayer::Callback(ma_device* dev, void* out, const void*, uint32_t frames) {
    auto* self = static_cast<AudioPlayer*>(dev->pUserData);

    ma_uint64 got = 0;
    ma_decoder_read_pcm_frames(self->m_Decoder, out, frames, &got);

    auto* f = static_cast<float *>(out);
    const uint64_t w = self->m_Written.load();

    for (ma_uint64 i = 0; i < got; i++) {
        f[i * 2]     *= self->m_Volume;
        f[i * 2 + 1] *= self->m_Volume;

        self->m_Recent[(w + i) % kSize] =
            0.5f * (f[i * 2] + f[i * 2 + 1]);
    }

    self->m_Written.store(w + got);
}

double AudioPlayer::GetElapsedTime() const { return static_cast<double>(m_Written.load()) / 44100.0; }

void AudioPlayer::GetLatestSamples(float* out, const size_t count) const {
    const uint64_t w = m_Written.load();
    if (w < count) { for (size_t i = 0; i < count; i++) out[i] = 0; return; }
    for (size_t i = 0; i < count; i++) out[i] = m_Recent[(w - count + i) % kSize];
}