//
// Created by crisel on 7/19/26.
//

#ifndef BLOCKGAME_IRENDERBACKEND_H
#define BLOCKGAME_IRENDERBACKEND_H
#include <cstdint>

struct MeshHandle {
    uint32_t handle;
};

struct TextureHandle {
    uint32_t handle;
};

struct ShaderHandle {
    uint32_t handle;
};

class IRenderBackend {
public:
    virtual ~IRenderBackend() = default;


};

#endif //BLOCKGAME_IRENDERBACKEND_H
