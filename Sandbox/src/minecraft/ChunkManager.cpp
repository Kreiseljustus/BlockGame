//
// Created by Kreiseljustus on 8/9/2026.
//

#include "ChunkManager.h"

using namespace Engine::Rendering;

namespace {
    int FloorDiv(int a, int b) {
        return (a >= 0) ? (a / b) : -((-a + b - 1) / b);
    }
}


bool ChunkManager::IsBlockSolid(const glm::ivec3 &worldBlockPos) const {
    const ChunkCoord coord = {
        FloorDiv(worldBlockPos.x, CHUNK_SIZE_XZ),
        FloorDiv(worldBlockPos.z, CHUNK_SIZE_XZ)
    };

    const auto it = m_Chunks.find(coord);
    if (it == m_Chunks.end()) return false;

    const int localX = worldBlockPos.x - coord.x * CHUNK_SIZE_XZ;
    const int localZ = worldBlockPos.z - coord.z * CHUNK_SIZE_XZ;

    const BlockType type = it->second.GetBlockAt(localX, worldBlockPos.y, localZ);
    return type != BlockType::Air && type != BlockType::GrassPlant;
}

GlobalPosToLocalBlockInfo ChunkManager::WorldToChunkPos(glm::ivec3 worldBlockPos) {
    const ChunkCoord coord = {
        FloorDiv(worldBlockPos.x, CHUNK_SIZE_XZ),
        FloorDiv(worldBlockPos.z, CHUNK_SIZE_XZ)
    };

    auto it = m_Chunks.find(coord);
    if (it == m_Chunks.end()) return {nullptr,{0,0,0}};

    const int localX = worldBlockPos.x - coord.x * CHUNK_SIZE_XZ;
    const int localZ = worldBlockPos.z - coord.z * CHUNK_SIZE_XZ;

    return {&it->second, {localX, worldBlockPos.y, localZ}};
}

void ChunkManager::Update(const glm::vec3 &playerPos, const Renderer &renderer, const siv::PerlinNoise& perlin) {
    const ChunkCoord center = WorldToChunkCoord(playerPos);

    for (int dx = -m_RenderDistance; dx <= m_RenderDistance; dx++) {
        for (int dz = -m_RenderDistance; dz <= m_RenderDistance; dz++) {
            ChunkCoord coord = {center.x + dx, center.z + dz};
            if (m_Chunks.find(coord) == m_Chunks.end()) {
                Chunk chunk;
                chunk.SetChunkCoords(coord.x, coord.z);
                GenerateTerrainForChunk(chunk,perlin);
                MeshData meshData = Chunk::GenerateChunkMesh(chunk);
                chunk.SetMesh(renderer.CreateMesh(meshData));
                chunk.hasMesh = true;
                m_Chunks.emplace(coord, std::move(chunk));
            }
        }
    }

    for (auto it = m_Chunks.begin(); it != m_Chunks.end();) {
        const int dx = it->first.x - center.x;
        const int dz = it->first.z - center.z;
        if (std::abs(dx) > m_RenderDistance || std::abs(dz) > m_RenderDistance) {
            if (it->second.hasMesh) renderer.DestroyMesh(it->second.GetMesh());
            it = m_Chunks.erase(it);
        } else {
            ++it;
        }
    }
}

void ChunkManager::Render(Renderer &renderer, ShaderHandle shader, TextureHandle texture) {
    for (auto& [coord, chunk] : m_Chunks) {
        Transform t;
        t.position = {coord.x * CHUNK_SIZE_XZ, 0, coord.z * CHUNK_SIZE_XZ};
        t.rotation = {0,0,0};
        t.scale = {1,1,1};
        renderer.Submit(chunk.GetMesh(), {shader, texture, false}, t);
    }
}

ChunkCoord ChunkManager::WorldToChunkCoord(const glm::vec3 &worldPos) {
    return {
    static_cast<int>(std::floor(worldPos.x / CHUNK_SIZE_XZ)),
    static_cast<int>(std::floor(worldPos.z / CHUNK_SIZE_XZ))
    };
}
