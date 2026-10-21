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

AVLayer::AVLayer() : m_Renderer(std::make_unique<Engine::Rendering::OpenGLBackend>()), m_Window(Application::Get().GetWindow()) {
    m_App = &Application::Get();
}

void AVLayer::OnAttach() {
    m_App->GetSceneManager().LoadScene<AudioVisualizer>();

    m_Camera.projection = ProjectionType::Perspective;
    m_Camera.position = {0,100,5};
    m_Camera.orthoSize = 3.0f;
    m_Camera.farPlane = 700;
    m_Camera.nearPlane = .3;
    m_Camera.aspect = static_cast<float>(m_Window.getFrameBufferSize().x) / static_cast<float>(m_Window.getFrameBufferSize().y);
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

void AVLayer::OnImGuiRender() {
    auto& sm = m_App->GetSceneManager();
    if (sm.HasActiveScene()) {
        if (auto* viz = dynamic_cast<AudioVisualizer*>(&sm.GetActiveScene()))
            viz->OnRenderImGui();
    }
}

