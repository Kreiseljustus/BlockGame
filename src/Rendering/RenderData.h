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

//hex codes from glew

enum TextureType : int {
    TEXTURE_2D = 0x0DE1,
    TEXTURE_3D = 0x806F,
    TEXTURE_CUBE_MAP = 0x8513,
    TEXTURE_2D_ARRAY = 0x8C1A
};

enum TextureWrapping : int {
    CLAMP = 0x2900,
    REPEAT = 0x2901
};

enum TextureFilter : int {
    NEAREST = 0x2600,
    LINEAR = 0x2601,
};

struct TextureParameters {
    TextureType type;
    TextureWrapping wrapping;
    TextureFilter filter;
    int width, height;
    int format;
    int dataType;

    unsigned char* imageData;
};

struct ShaderParameters {
    const std::filesystem::path& vertexShader;
    const std::filesystem::path& fragmentShader;
};

struct MeshHandle {
    uint32_t handle;
};

struct TextureHandle {
    uint32_t handle;
};

struct ShaderHandle {
    uint32_t handle;
};

struct Texture {
    TextureParameters params;
    TextureHandle tHandle;
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
