//
// Created by crisel on 7/20/26.
//

#ifndef BLOCKGAME_OPENGLBACKEND_H
#define BLOCKGAME_OPENGLBACKEND_H

#include "GL/glew.h"
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
        ShaderHandle CreateShader(const ShaderParameters& parameters) override;

    private:
        struct GPUMesh {
            GLuint vao = 0;
            GLuint vbo = 0;
            GLuint ebo = 0;
            uint32_t indexCount = 0;
        };

        std::vector<GPUMesh> m_Meshes;
    };
}


#endif //BLOCKGAME_OPENGLBACKEND_H
