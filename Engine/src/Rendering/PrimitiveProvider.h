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
        // Position                                          Normal                           UV
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
    using namespace Engine::Rendering;

    const std::vector<Vertex> vertices = {
        // +X face (right)
        {{ 1.0f, -1.0f, -1.0f}, {1, 0, 0}, {0, 0}},
        {{ 1.0f,  1.0f, -1.0f}, {1, 0, 0}, {0, 1}},
        {{ 1.0f,  1.0f,  1.0f}, {1, 0, 0}, {1, 1}},
        {{ 1.0f, -1.0f,  1.0f}, {1, 0, 0}, {1, 0}},

        // -X face (left)
        {{-1.0f, -1.0f,  1.0f}, {-1, 0, 0}, {0, 0}},
        {{-1.0f,  1.0f,  1.0f}, {-1, 0, 0}, {0, 1}},
        {{-1.0f,  1.0f, -1.0f}, {-1, 0, 0}, {1, 1}},
        {{-1.0f, -1.0f, -1.0f}, {-1, 0, 0}, {1, 0}},

        // +Y face (top)
        {{-1.0f,  1.0f, -1.0f}, {0, 1, 0}, {0, 0}},
        {{-1.0f,  1.0f,  1.0f}, {0, 1, 0}, {0, 1}},
        {{ 1.0f,  1.0f,  1.0f}, {0, 1, 0}, {1, 1}},
        {{ 1.0f,  1.0f, -1.0f}, {0, 1, 0}, {1, 0}},

        // -Y face (bottom)
        {{-1.0f, -1.0f,  1.0f}, {0, -1, 0}, {0, 0}},
        {{-1.0f, -1.0f, -1.0f}, {0, -1, 0}, {0, 1}},
        {{ 1.0f, -1.0f, -1.0f}, {0, -1, 0}, {1, 1}},
        {{ 1.0f, -1.0f,  1.0f}, {0, -1, 0}, {1, 0}},

        // +Z face (front)
        {{-1.0f, -1.0f,  1.0f}, {0, 0, 1}, {0, 0}},
        {{ 1.0f, -1.0f,  1.0f}, {0, 0, 1}, {1, 0}},
        {{ 1.0f,  1.0f,  1.0f}, {0, 0, 1}, {1, 1}},
        {{-1.0f,  1.0f,  1.0f}, {0, 0, 1}, {0, 1}},

        // -Z face (back)
        {{ 1.0f, -1.0f, -1.0f}, {0, 0, -1}, {0, 0}},
        {{-1.0f, -1.0f, -1.0f}, {0, 0, -1}, {1, 0}},
        {{-1.0f,  1.0f, -1.0f}, {0, 0, -1}, {1, 1}},
        {{ 1.0f,  1.0f, -1.0f}, {0, 0, -1}, {0, 1}},
    };

    const std::vector<uint32_t> indices = {
        0, 2, 1,   0, 3, 2,       // +X
        4, 6, 5,   4, 7, 6,       // -X
        8, 10, 9,  8, 11, 10,     // +Y
        12, 14, 13, 12, 15, 14,   // -Y
        16, 18, 17, 16, 19, 18,   // +Z
        20, 22, 21, 20, 23, 22    // -Z
    };

    return { vertices, indices };
}

#endif //BLOCKGAME_PRIMITIVEPROVIER_H
