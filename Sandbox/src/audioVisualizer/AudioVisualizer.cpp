//
// Created by Kreiseljustus on 10/21/2026.
//

#include "AudioVisualizer.h"

#include <iostream>

#include "Analyzer.h"
#include "ECS/BasicComponents.h"

#include "imgui.h"

AudioVisualizer::~AudioVisualizer() = default;

void AudioVisualizer::OnRenderImGui() {
    float samples[Analyzer::N];
    m_Music.GetComponent<Engine::Components::AudioSourceComponent>().GetLatestSamples(samples, Analyzer::N);
    m_Analyzer.Update(samples);
    ImGui::PlotHistogram("test", m_Analyzer.bands.data(), Analyzer::BANDS, 0, nullptr, 0.0f, 1.0f, ImVec2(400, 100));
}

void AudioVisualizer::OnSceneUpdate() {

}

void AudioVisualizer::OnLoad() {
    m_Analyzer = Analyzer();

    m_Music = CreateGameObject();
    auto& audioComponent = m_Music.AddComponent<Engine::Components::AudioSourceComponent>();
    bool sucess = audioComponent.Load("assets/audio/test.mp3");
    std::cout << "Successfully loaded? " << sucess << std::endl;
    audioComponent.SetVolume(0.1f);
    audioComponent.SetLooping(true);
    audioComponent.Play();
}
