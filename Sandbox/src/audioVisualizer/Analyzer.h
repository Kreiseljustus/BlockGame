//
// Created by Kreiseljustus on 10/21/2026.
//

#ifndef BLOCKGAME_ANALYZER_H
#define BLOCKGAME_ANALYZER_H

#include <vector>
#include <complex>
#include <cmath>
#include <algorithm>

//MATH (not written by me)
//https://en.wikipedia.org/wiki/Fourier_transform
inline void FFT(std::vector<std::complex<float>>& a) {
    size_t n = a.size();
    for (size_t i = 1, j = 0; i < n; i++) {
        size_t bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) std::swap(a[i], a[j]);
    }
    for (size_t len = 2; len <= n; len <<= 1) {
        float ang = -2.0f * 3.14159265f / len;
        std::complex<float> wl(std::cos(ang), std::sin(ang));
        for (size_t i = 0; i < n; i += len) {
            std::complex<float> w(1);
            for (size_t j = 0; j < len / 2; j++) {
                auto u = a[i + j], v = a[i + j + len/2] * w;
                a[i + j] = u + v;
                a[i + j + len/2] = u - v;
                w *= wl;
            }
        }
    }
}

class Analyzer {
public:
    static constexpr int N = 2048; //Amount of samples to look at
    static constexpr int BANDS = 32; //Amount of bars for visualizer
    std::vector<float> bands = std::vector<float>(BANDS,0.0f);

    void Update(const float* samples) {
        std::vector<std::complex<float>> data(N);
        for (int i = 0; i < N; i++) {
            float fade = 0.5f - 0.5f * std::cos(2.0f * 3.14159265f * i / (N - 1));
            data[i] = samples[i] * fade;
        }

        FFT(data);

        float binHz = 44100.0f / N;
        for (int b = 0; b < BANDS; b++) {
            float f0 = 40.0f    * std::pow(16000.0f / 40.0f, float(b)     / BANDS);
            float f1 = 40.0f    * std::pow(16000.0f / 40.0f, float(b + 1) / BANDS);
            int k0 = int(f0 / binHz);
            int k1 = std::max(k0 + 1, int(f1 / binHz));

            float loudest = 0;
            for (int k = k0; k < k1; k++)
                loudest = std::max(loudest, std::abs(data[k]) * 2.0f / N);

            float value = std::min(1.0f, std::log1p(30.0f * loudest) / std::log1p(30.0f));

            if (value > bands[b]) bands[b] = value;
            else                  bands[b] = bands[b] * 0.88f + value * 0.12f;
        }
    }
};

#endif //BLOCKGAME_ANALYZER_H
