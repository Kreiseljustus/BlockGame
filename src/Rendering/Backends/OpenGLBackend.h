//
// Created by crisel on 7/20/26.
//

#ifndef BLOCKGAME_OPENGLBACKEND_H
#define BLOCKGAME_OPENGLBACKEND_H

#include "Rendering/IRenderBackend.h"
#include "Rendering/RenderData.h"

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
};


#endif //BLOCKGAME_OPENGLBACKEND_H
