//
// Created by crisel on 7/20/26.
//

#include "Renderer.h"

#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace Engine::Rendering;

glm::mat4 GetViewMatrix(const Camera& cam) {
    auto view = glm::mat4(1.0f);
    view = glm::rotate(view, glm::radians(-cam.rotation.x), glm::vec3(1, 0, 0));
    view = glm::rotate(view, glm::radians(-cam.rotation.y), glm::vec3(0, 1, 0));
    view = glm::rotate(view, glm::radians(-cam.rotation.z), glm::vec3(0, 0, 1));
    view = glm::translate(view, -cam.position);
    return view;
}

glm::mat4 GetProjectionMatrix(const Camera& cam) {
    if (cam.projection == ProjectionType::Perspective) {
        return glm::perspective(glm::radians(cam.fov), cam.aspect, cam.nearPlane, cam.farPlane);
    } else {
        const float halfHeight = cam.orthoSize;
        const float halfWidth = halfHeight * cam.aspect;
        return glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, cam.nearPlane, cam.farPlane);
    }
}

void Engine::Rendering::Renderer::Submit(const MeshHandle mesh, const Material material, const Transform& transform) {
    m_DrawCommands.emplace_back(DrawCommand{mesh, material, transform});
}

void Engine::Rendering::Renderer::DrawLine(glm::vec3 a, glm::vec3 b, Color color) const {
    m_RenderBackend->DrawDynamicBuffer(nullptr, 0, TRIANGLES);
}

void Engine::Rendering::Renderer::Begin(const Camera& camera) {
    m_DrawCommands.clear();
    m_DrawCommands.reserve(100);

    m_RenderBackend->SetViewProjection(GetViewMatrix(camera), GetProjectionMatrix(camera));

    m_RenderBackend->Begin();
}

void Engine::Rendering::Renderer::End() {
    Flush();
    m_RenderBackend->End();
}

void Engine::Rendering::Renderer::Flush() {
    std::sort(m_DrawCommands.begin(), m_DrawCommands.end(), [](const DrawCommand& a, const DrawCommand& b) {
        return a.material.shaderHandle < b.material.shaderHandle;
    });

    for (const auto& cmd : m_DrawCommands)
        m_RenderBackend->Draw(cmd.mesh, cmd.material, cmd.transform);

    m_DrawCommands.clear();
}
