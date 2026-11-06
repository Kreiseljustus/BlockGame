//
// Created by Kreiseljustus on 10/21/2026.
//

#include "AstroLayer.h"

#include "AstroScene.h"
#include "Rendering/Renderer.h"
#include "Rendering/Backends/OpenGLBackend.h"

using namespace Engine;
using namespace Engine::Core;

AstroLayer::AstroLayer() : m_Window(Application::Get().GetWindow()) {
    m_App = &Application::Get();
    input = m_App->GetInput();
}

void AstroLayer::OnAttach() {
   m_App->GetSceneManager().LoadScene<AstroScene>(m_App->GetRenderer());
}

void AstroLayer::OnUpdate(float deltaTime) {

}

void AstroLayer::OnRender() {

}

void AstroLayer::OnImGuiRender() {

}

