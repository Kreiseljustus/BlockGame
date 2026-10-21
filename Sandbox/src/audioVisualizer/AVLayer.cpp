//
// Created by Kreiseljustus on 10/21/2026.
//

#include "AVLayer.h"

#include <memory>

#include "AudioVisualizer.h"
#include "Rendering/Renderer.h"
#include "Rendering/Backends/OpenGLBackend.h"

using namespace Engine;
using namespace Engine::Core;

AVLayer::AVLayer() : m_Renderer(std::make_unique<Engine::Rendering::OpenGLBackend>()) {
    m_App = &Application::Get();
}

void AVLayer::OnAttach() {
    m_App->GetSceneManager().LoadScene<AudioVisualizer>();
}

void AVLayer::OnUpdate(float deltaTime) {
}

void AVLayer::OnRender() {
    auto& sceneManager = m_App->GetSceneManager();

    m_Renderer.Begin(m_Camera);
    if (sceneManager.HasActiveScene()) {
        sceneManager.OnRender(m_Renderer);
    }
    m_Renderer.End();
}
