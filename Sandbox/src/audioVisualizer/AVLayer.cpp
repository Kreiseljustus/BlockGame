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
    input = m_App->GetInput();
}

void AVLayer::OnAttach() {
    m_App->GetSceneManager().LoadScene<AudioVisualizer>(m_Renderer);

    m_Camera.projection = ProjectionType::Perspective;
    m_Camera.position = {0,5,20};
    m_Camera.orthoSize = 3.0f;
    m_Camera.farPlane = 700;
    m_Camera.nearPlane = .3;
    m_Camera.aspect = static_cast<float>(m_Window.getFrameBufferSize().x) / static_cast<float>(m_Window.getFrameBufferSize().y);
}

void AVLayer::OnUpdate(float deltaTime) {

    if (input->IsMouseButtonDown(1)) {
        const float lookSensitivity = 0.15f;
        float camSpeed = 12.0f;

        glm::vec2 mouseDelta = input->ConsumeMouseDelta();
        m_Camera.rotation.y -= mouseDelta.x * lookSensitivity; // yaw
        m_Camera.rotation.x -= mouseDelta.y * lookSensitivity; // pitch
        m_Camera.rotation.x = std::clamp(m_Camera.rotation.x, -89.0f, 89.0f);

        float yawRad = glm::radians(m_Camera.rotation.y);
        float pitchRad = glm::radians(m_Camera.rotation.x);

        glm::vec3 forward = {
            -sinf(yawRad) * cosf(pitchRad),
            sinf(pitchRad),
            -cosf(yawRad) * cosf(pitchRad)
        };
        glm::vec3 right = {cosf(yawRad), 0, -sinf(yawRad)};

        if (input->IsKeyDown(GLFW_KEY_W)) m_Camera.position += forward * camSpeed * deltaTime;
        if (input->IsKeyDown(GLFW_KEY_S)) m_Camera.position -= forward * camSpeed * deltaTime;
        if (input->IsKeyDown(GLFW_KEY_A)) m_Camera.position -= right * camSpeed * deltaTime;
        if (input->IsKeyDown(GLFW_KEY_D)) m_Camera.position += right * camSpeed * deltaTime;
    }

    input->ConsumeMouseDelta();
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

