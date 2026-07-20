//
// Created by crisel on 7/19/26.
//

#ifndef BLOCKGAME_IRENDERBACKEND_H
#define BLOCKGAME_IRENDERBACKEND_H

#include "Rendering/RenderData.h"

class IRenderBackend {
public:
    virtual ~IRenderBackend() = default;

    virtual void Begin() = 0;
    virtual void End() = 0;

    virtual void Draw(MeshHandle) = 0;
    virtual void DrawDynamicBuffer(void* vertexData, int count, PrimitiveType type) = 0;

    virtual MeshHandle CreateMesh(const MeshData& data) = 0;
    //TODO
    virtual TextureHandle CreateTexture() = 0;
    virtual ShaderHandle CreateShader() = 0;
};

#endif //BLOCKGAME_IRENDERBACKEND_H
