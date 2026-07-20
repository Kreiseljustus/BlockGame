//
// Created by crisel on 7/20/26.
//

#ifndef BLOCKGAME_RENDERDATA_H
#define BLOCKGAME_RENDERDATA_H
#include <cstdint>
#include <vector>

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

struct MeshHandle {
    uint32_t handle;
};

struct TextureHandle {
    uint32_t handle;
};

struct ShaderHandle {
    uint32_t handle;
};

struct Material {
    uint32_t shaderHandle;
    uint32_t textureHandle;
    //Color and other stuff
};

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
};

struct MeshData {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
};

struct Position3 {
    glm::vec3 xyz;
};

struct Transform {
    Position3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
};

struct Color {
    glm::vec4 rgba;
};

enum PrimitiveType {
    LINES = 0,
    TRIANGLES
};

#endif //BLOCKGAME_RENDERDATA_H
