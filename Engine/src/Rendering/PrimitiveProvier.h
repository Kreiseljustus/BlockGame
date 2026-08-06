//
// Created by Kreiseljustus on 8/6/2026.
//

#ifndef BLOCKGAME_PRIMITIVEPROVIER_H
#define BLOCKGAME_PRIMITIVEPROVIER_H
#include "RenderData.h"

inline Engine::Rendering::MeshData GetUnitQuad()
{
    const std::vector<Engine::Rendering::Vertex> vertices =
    {
        // Position                  Normal                  UV
        {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}}, // Bottom-left
        {{ 0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}}, // Bottom-right
        {{ 0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}}, // Top-right
        {{-0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}}, // Top-left
    };

    const std::vector<uint32_t> indices =
    {
        0, 1, 2,
        2, 3, 0
    };

    return { vertices, indices };
}

inline Engine::Rendering::MeshData GetUnitCube() {
    return {};
}

#endif //BLOCKGAME_PRIMITIVEPROVIER_H
