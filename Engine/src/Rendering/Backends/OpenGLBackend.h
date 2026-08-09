//
// Created by crisel on 7/20/26.
//

#ifndef BLOCKGAME_OPENGLBACKEND_H
#define BLOCKGAME_OPENGLBACKEND_H

#include <unordered_set>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Rendering/IRenderBackend.h"
#include "Rendering/RenderData.h"

namespace Engine::Rendering {
    class OpenGLBackend : public IRenderBackend{
    public:
        OpenGLBackend();

        void Begin() override;
        void End() override;
        void Draw(MeshHandle, Material, Transform) override;
        void DrawDynamicBuffer(void *vertexData, int count, PrimitiveType type) override;

        MeshHandle CreateMesh(const MeshData &data) override;
        TextureHandle CreateTexture(const TextureParameters& parameters) override;

        //Unused
        ShaderHandle CreateShader(const ShaderParameters& parameters) override;

        void UpdateTexture(TextureHandle handle, const void* pixelData) override;
        void UpdateMesh(MeshHandle handle, const MeshData &data) override;

        void DestroyMesh(MeshHandle handle) override;

        void SetViewProjection(const glm::mat4& view, const glm::mat4& proj) override;

    private:
        struct GPUMesh {
            GLuint vao = 0;
            GLuint vbo = 0;
            GLuint ebo = 0;
            uint32_t indexCount = 0;
            uint32_t generation = 0;
            bool alive = false;
        };

        struct GPUTexture {
            GLuint id = 0;
            int width = 0;
            int height = 0;
            GLenum format = GL_RGBA;
        };

        std::vector<GPUMesh> m_Meshes;
        std::vector<uint32_t> m_FreeMeshSlots;

        std::vector<GPUTexture> m_Textures;

        glm::mat4 m_View = glm::mat4(1.0f);
        glm::mat4 m_Proj = glm::mat4(1.0f);

        std::unordered_set<GLuint> m_ShadersUpdatedCurrentFrame;
    };
}


#endif //BLOCKGAME_OPENGLBACKEND_H
