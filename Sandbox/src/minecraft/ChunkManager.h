//
// Created by Kreiseljustus on 8/9/2026.
//

#ifndef BLOCKGAME_CHUNKMANAGER_H
#define BLOCKGAME_CHUNKMANAGER_H

#include "Chunk.h"
#include "Rendering/Renderer.h"

#include <unordered_map>
#include <glm/vec3.hpp>

struct GlobalPosToLocalBlockInfo {
    Chunk* chunk;
    glm::vec3 localCoords;
};

class ChunkManager {
public:
    bool IsBlockSolid(const glm::ivec3& worldBlockPos) const;
    GlobalPosToLocalBlockInfo WorldToChunkPos(glm::ivec3 worldPos);
    void Update(const glm::vec3& playerPos, const Engine::Rendering::Renderer& renderer, const siv::PerlinNoise& perlin);
    void Render(Engine::Rendering::Renderer& renderer, Engine::Rendering::ShaderHandle shader, Engine::Rendering::TextureHandle texture);
private:
    static ChunkCoord WorldToChunkCoord(const glm::vec3& worldPos);
private:
    std::unordered_map<ChunkCoord, Chunk, ChunkCoordHash> m_Chunks;
    int m_RenderDistance = 20;
};


#endif //BLOCKGAME_CHUNKMANAGER_H
