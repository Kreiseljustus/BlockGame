//
// Created by Kreiseljustus on 10/21/2026.
//

#include "AudioVisualizer.h"

#include <iostream>

#include "Analyzer.h"
#include "ECS/BasicComponents.h"

#include "imgui.h"
#include "ResourceManager.h"
#include "Rendering/PrimitiveProvider.h"

AudioVisualizer::AudioVisualizer(Engine::Rendering::Renderer& r) : Scene(r) {
    renderer = &r;
}

AudioVisualizer::~AudioVisualizer() = default;

void AudioVisualizer::OnRenderImGui() {
    ImGui::PlotHistogram("test", m_Analyzer.bands.data(), Analyzer::BANDS, 0, nullptr, 0.0f, 1.0f, ImVec2(400, 100));
}

void AudioVisualizer::OnSceneUpdate() {
    float samples[Analyzer::N];
    m_Music.GetComponent<Engine::Components::AudioSourceComponent>().GetLatestSamples(samples, Analyzer::N);
    m_Analyzer.Update(samples);

    for (int i = 0; i < Analyzer::BANDS; i++) {
        auto& t = m_AudioBars[i].GetComponent<Engine::Rendering::Transform>();
        float h = 0.1f + m_Analyzer.bands[i] * 8.0f;
        t.scale      = { 0.4f, h, 0.4f };
        t.position.y = h * 0.5f;
    }
}

void AudioVisualizer::OnLoad() {

    m_Cube = renderer->CreateMesh(GetUnitCube());
    Engine::Rendering::ShaderHandle a = Engine::ResourceManager::LoadShader("assets/shaders/BasicVertexShader.glsl", "assets/shaders/BasicFragmentShader.glsl", "basic");
    Engine::Rendering::TextureParameters c = Engine::Rendering::TextureParameters();
    Engine::ResourceManager::LoadTexture(c, "assets/textures/dirt.png", "b");
    Engine::Rendering::TextureHandle b = renderer->CreateTexture(Engine::ResourceManager::LoadTexture(c, "assets/textures/dirt.png", "b"));
    m_Material = {a,b,false};

    m_Analyzer = Analyzer();

    m_Music = CreateGameObject();
    auto& audioComponent = m_Music.AddComponent<Engine::Components::AudioSourceComponent>();
    bool sucess = audioComponent.Load("assets/audio/know.mp3");
    std::cout << "Successfully loaded? " << sucess << std::endl;
    audioComponent.SetVolume(0.1f);
    audioComponent.SetLooping(true);
    audioComponent.Play();

    for (int i = 0; i < Analyzer::BANDS; i++) {
        auto bar = CreateGameObject("Bar" + std::to_string(i));
        bar.GetComponent<Engine::Rendering::Transform>().position =
            { (i - Analyzer::BANDS / 2) * 0.5f, 0.0f, 0.0f };
        bar.AddComponent<Engine::Components::MeshRendererComponent>(m_Cube, m_Material);
        m_AudioBars.push_back(bar);
    }
}
