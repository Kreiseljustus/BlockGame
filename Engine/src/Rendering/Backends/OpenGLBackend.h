//
// Created by crisel on 7/20/26.
//

#ifndef BLOCKGAME_OPENGLBACKEND_H
#define BLOCKGAME_OPENGLBACKEND_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Rendering/IRenderBackend.h"
#include "Rendering/RenderData.h"

namespace Engine::Rendering {
    class OpenGLBackend : public IRenderBackend{
    public:
        OpenGLBackend() = default;

        void Begin() override;
        void End() override;
        void Draw(MeshHandle, Material, Transform) override;
        void DrawDynamicBuffer(void *vertexData, int count, PrimitiveType type) override;

        MeshHandle CreateMesh(const MeshData &data) override;
        TextureHandle CreateTexture(const TextureParameters& parameters) override;

        //Unused
        ShaderHandle CreateShader(const ShaderParameters& parameters) override;

        void UpdateTexture(TextureHandle handle, const void* pixelData) override;

        void SetViewProjection(const glm::mat4& view, const glm::mat4& proj) override;

    private:
        struct GPUMesh {
            GLuint vao = 0;
            GLuint vbo = 0;
            GLuint ebo = 0;
            uint32_t indexCount = 0;
        };

        struct GPUTexture {
            GLuint id = 0;
            int width = 0;
            int height = 0;
            GLenum format = GL_RGBA;
        };

        std::vector<GPUMesh> m_Meshes;
        std::vector<GPUTexture> m_Textures;

        glm::mat4 m_View = glm::mat4(1.0f);
        glm::mat4 m_Proj = glm::mat4(1.0f);
    };
}


#endif //BLOCKGAME_OPENGLBACKEND_H
