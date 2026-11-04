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

AVLayer::AVLayer() : m_Window(Application::Get().GetWindow()) {
    m_App = &Application::Get();
    input = m_App->GetInput();
}

void AVLayer::OnAttach() {
    m_App->GetSceneManager().LoadScene<AudioVisualizer>(m_App->GetRenderer());
}

void AVLayer::OnUpdate(float deltaTime) {

}

void AVLayer::OnRender() {

}

void AVLayer::OnImGuiRender() {
    auto& sm = m_App->GetSceneManager();
    if (sm.HasActiveScene()) {
        if (auto* viz = dynamic_cast<AudioVisualizer*>(&sm.GetActiveScene()))
            viz->OnRenderImGui();
    }
}

