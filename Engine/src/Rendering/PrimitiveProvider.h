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
        //+X face (right)
        {{ 0.5f, -0.5f, -0.5f}, {1, 0, 0}, {0, 0}},
        {{ 0.5f,  0.5f, -0.5f}, {1, 0, 0}, {0, 1}},
        {{ 0.5f,  0.5f,  0.5f}, {1, 0, 0}, {1, 1}},
        {{ 0.5f, -0.5f,  0.5f}, {1, 0, 0}, {1, 0}},

        //-X face (left)
        {{-0.5f, -0.5f,  0.5f}, {-1, 0, 0}, {0, 0}},
        {{-0.5f,  0.5f,  0.5f}, {-1, 0, 0}, {0, 1}},
        {{-0.5f,  0.5f, -0.5f}, {-1, 0, 0}, {1, 1}},
        {{-0.5f, -0.5f, -0.5f}, {-1, 0, 0}, {1, 0}},

        //+Y face (top)
        {{-0.5f,  0.5f, -0.5f}, {0, 1, 0}, {0, 0}},
        {{-0.5f,  0.5f,  0.5f}, {0, 1, 0}, {0, 1}},
        {{ 0.5f,  0.5f,  0.5f}, {0, 1, 0}, {1, 1}},
        {{ 0.5f,  0.5f, -0.5f}, {0, 1, 0}, {1, 0}},

        //-Y face (bottom)
        {{-0.5f, -0.5f,  0.5f}, {0, -1, 0}, {0, 0}},
        {{-0.5f, -0.5f, -0.5f}, {0, -1, 0}, {0, 1}},
        {{ 0.5f, -0.5f, -0.5f}, {0, -1, 0}, {1, 1}},
        {{ 0.5f, -0.5f,  0.5f}, {0, -1, 0}, {1, 0}},

        //+Z face (front)
        {{-0.5f, -0.5f,  0.5f}, {0, 0, 1}, {0, 0}},
        {{ 0.5f, -0.5f,  0.5f}, {0, 0, 1}, {1, 0}},
        {{ 0.5f,  0.5f,  0.5f}, {0, 0, 1}, {1, 1}},
        {{-0.5f,  0.5f,  0.5f}, {0, 0, 1}, {0, 1}},

        //-Z face (back)
        {{ 0.5f, -0.5f, -0.5f}, {0, 0, -1}, {0, 0}},
        {{-0.5f, -0.5f, -0.5f}, {0, 0, -1}, {1, 0}},
        {{-0.5f,  0.5f, -0.5f}, {0, 0, -1}, {1, 1}},
        {{ 0.5f,  0.5f, -0.5f}, {0, 0, -1}, {0, 1}},
    };

    const std::vector<uint32_t> indices = {
        0, 1, 2,   0, 2, 3,    // +X
        4, 5, 6,   4, 6, 7,    // -X
        8, 9, 10,  8, 10, 11,  // +Y
        12, 13, 14, 12, 14, 15, // -Y
        16, 17, 18, 16, 18, 19, // +Z
        20, 21, 22, 20, 22, 23  // -Z
    };

    return { vertices, indices };
}

#endif //BLOCKGAME_PRIMITIVEPROVIER_H
