//
// Created by crisel on 7/20/26.
//

#ifndef BLOCKGAME_RENDERDATA_H
#define BLOCKGAME_RENDERDATA_H
#include <cstdint>
#include <vector>
#include <filesystem>

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include <glm/glm.hpp>

#include <GL/glew.h>

//hex codes from glew

namespace Engine::Rendering {

    enum class ProjectionType {
        Perspective,
        Orthographic
    };

    struct Camera {
        glm::vec3 position = {0,0,0};
        glm::vec3 rotation = {0,0,0};

        ProjectionType projection = ProjectionType::Orthographic;

        //perspective only
        float fov = 60.0f;
        float aspect = 800.0f / 600.0f;

        //ortho only
        float orthoSize = 10.0f;

        float nearPlane = 0.1f;
        float farPlane = 1000.0f;
    };

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
        TextureType type = TEXTURE_2D;
        TextureWrapping wrapping = CLAMP;
        TextureFilter filter = NEAREST;
        int width = 0, height = 0;
        int format = GL_RGBA;
        int dataType = GL_UNSIGNED_BYTE;

        unsigned char* imageData = nullptr;
    };

    struct ShaderParameters {
        const std::filesystem::path vertexShader = "vertexShader.glsl";
        const std::filesystem::path fragmentShader = "fragmentShader.glsl";
    };

    struct MeshHandle {
        uint32_t index = 0;
        uint32_t generation = 0;
    };

    struct TextureHandle {
        uint32_t handle = 0;
    };

    struct ShaderHandle {
        uint32_t handle = 0;
    };

    struct Texture {
        TextureParameters params = {};
        TextureHandle tHandle = {0};
    };

    struct Material {
        ShaderHandle shaderHandle = {0};
        TextureHandle textureHandle = {0};
        //Color and other stuff
    };

    struct Vertex {
        glm::vec3 position {0,0,0};
        glm::vec3 normal {1,1,1};
        glm::vec2 uv {0,0};
    };

    struct MeshData {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
    };

    struct Transform {
        glm::vec3 position = {0,0,0};
        glm::vec3 rotation = {0,0,0};
        glm::vec3 scale = {1,1,1};
    };

    struct Color {
        glm::vec4 rgba = {255,255,255,255};
    };

    enum PrimitiveType {
        LINES = 0,
        TRIANGLES
    };
}

#endif //BLOCKGAME_RENDERDATA_H
