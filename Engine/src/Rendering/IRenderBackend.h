//
// Created by crisel on 7/19/26.
//

#ifndef BLOCKGAME_IRENDERBACKEND_H
#define BLOCKGAME_IRENDERBACKEND_H

#include "Rendering/RenderData.h"

namespace Engine::Rendering {
    class IRenderBackend {
    public:
        virtual ~IRenderBackend() = default;

        virtual void Begin() = 0;
        virtual void End() = 0;

        virtual void Draw(MeshHandle, Material, Transform) = 0;
        virtual void DrawDynamicBuffer(void* vertexData, int count, PrimitiveType type) = 0;

        virtual void UpdateTexture(TextureHandle handle, const void* pixelData) = 0;
        virtual void SetViewProjection(const glm::mat4& view, const glm::mat4& proj) = 0;

        virtual MeshHandle CreateMesh(const MeshData& data) = 0;
        virtual TextureHandle CreateTexture(const TextureParameters& parameters) = 0;

        //TODO: Actually use this
        //See Shader class (only OpenGl cause i'm stupid)
        virtual ShaderHandle CreateShader(const ShaderParameters& parameters) = 0;
    };
}

#endif //BLOCKGAME_IRENDERBACKEND_H
