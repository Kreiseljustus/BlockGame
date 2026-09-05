//
// Created by Kreiseljustus on 8/9/2026.
//

#ifndef BLOCKGAME_CHUNKMANAGER_H
#define BLOCKGAME_CHUNKMANAGER_H

#include "Chunk.h"
#include "Rendering/Renderer.h"

#include <unordered_map>

class ChunkManager {
public:
    void Update(const glm::vec3& playerPos, Engine::Rendering::Renderer& renderer, const siv::PerlinNoise& perlin);
    void Render(Engine::Rendering::Renderer& renderer, Engine::Rendering::ShaderHandle shader, Engine::Rendering::TextureHandle texture);
private:
    static ChunkCoord WorldToChunkCoord(const glm::vec3& worldPos);
private:
    std::unordered_map<ChunkCoord, Chunk, ChunkCoordHash> m_Chunks;
    int m_RenderDistance = 2;
};


#endif //BLOCKGAME_CHUNKMANAGER_H
