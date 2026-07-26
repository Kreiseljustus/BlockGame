//
// Created by crisel on 7/20/26.
//

#include "Renderer.h"

#include <algorithm>

void Renderer::Submit(const MeshHandle mesh, const Material material, const Transform transform) {
    m_DrawCommands.emplace_back(DrawCommand{mesh, material, transform});
}

void Renderer::DrawLine(Position3 a, Position3 b, Color color) const {
    m_RenderBackend->DrawDynamicBuffer(nullptr, 0, TRIANGLES);
}

void Renderer::Begin() {
    m_DrawCommands.clear();
    m_DrawCommands.reserve(100);

    m_RenderBackend->Begin();
}

void Renderer::End() {
    Flush();
    m_RenderBackend->End();
}

void Renderer::Flush() {
    std::sort(m_DrawCommands.begin(), m_DrawCommands.end(), [](const DrawCommand& a, const DrawCommand& b) {
        return a.material.shaderHandle < b.material.shaderHandle;
    });

    for (const auto& cmd : m_DrawCommands)
        m_RenderBackend->Draw(cmd.mesh, cmd.material, cmd.transform);

    m_DrawCommands.clear();
}
