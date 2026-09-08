//
// Created by Kreiseljustus on 8/9/2026.
//

#ifndef BLOCKGAME_CHUNK_H
#define BLOCKGAME_CHUNK_H

#include <cstdint>
#include "../utils/PerlinNoise.hpp"
#include "Rendering/RenderData.h"

#include "../utils/Utils.h"

constexpr int CHUNK_SIZE_XZ = 16;
constexpr int CHUNK_SIZE_Y = 64;

constexpr int ATLAS_TILES_PER_ROW = 4;
constexpr float ATLAS_TILE_SIZE = 1.0f / ATLAS_TILES_PER_ROW;

enum class BlockType : uint8_t {Air, Stone, Dirt, Grass, Log, Leave, GrassPlant};

inline glm::ivec2 GetAtlasTile(const BlockType type) {
    switch (type) {
        case BlockType::Dirt: {
            return {1,0};
            break;
        }
        case BlockType::Stone: {
            return {2,0};
            break;
        }
        case BlockType::Grass: {
            return {3,0};
            break;
        }
        case BlockType::Log: {
            return {0,1};
            break;
        }
        case BlockType::Leave: {
            return {1,1};
            break;
        }
        case BlockType::GrassPlant: {
            return {2,1};
            break;
        }
        default: {
            return {0,0};
            break;
        }
    }
}

inline void GetTileUVs(BlockType type, glm::vec2 outUVs[4]) {
    const glm::ivec2 tile = GetAtlasTile(type);
    float u0 = tile.x * ATLAS_TILE_SIZE;
    float v0 = tile.y * ATLAS_TILE_SIZE;
    float u1 = u0 + ATLAS_TILE_SIZE;
    float v1 = v0 + ATLAS_TILE_SIZE;

    outUVs[0] = {u0,v0};
    outUVs[1] = {u1,v0};
    outUVs[2] = {u1,v1};
    outUVs[3] = {u0,v1};
}

struct ChunkCoord {
    int x, z;

    bool operator==(const ChunkCoord& other) const {
        return x == other.x && z == other.z;
    }
};

struct ChunkCoordHash {
    size_t operator()(const ChunkCoord& c) const {
        return std::hash<int64_t>()((static_cast<int64_t>(c.x) << 32) ^ static_cast<uint32_t>(c.z));
    }
};

class Chunk {
public:
    Chunk() {
        m_Blocks.resize(CHUNK_SIZE_XZ * CHUNK_SIZE_Y * CHUNK_SIZE_XZ);
    }

    void SetBlockAt(int x, int y, int z, BlockType type);
    void SetChunkCoords(const int x, const int z) {cord.x = x; cord.z = z;}

    void SetMesh(const Engine::Rendering::MeshHandle handle) {mesh = handle;}

    [[nodiscard]] BlockType GetBlockAt(int x, int y, int z) const;

    [[nodiscard]] bool HasMesh() const {return hasMesh;}
    [[nodiscard]] ChunkCoord GetCoords() const {return cord;}
    [[nodiscard]] Engine::Rendering::MeshHandle GetMesh() const {return mesh;}

    static int GetSurfaceHeight(const int worldX, const int worldZ, const siv::PerlinNoise& perlin) {return static_cast<int>(perlin.noise2D_01(worldX*0.01f, worldZ * 0.01f) * (CHUNK_SIZE_Y - 1));}

    static Engine::Rendering::MeshData GenerateChunkMesh(const Chunk& chunk);

    static int Get3DTO1DBlockIndex(int x, int y, int z);
public:
    bool hasMesh = false;
private:
    Engine::Rendering::MeshHandle mesh {};
    ChunkCoord cord = {0,0};

    std::vector<BlockType> m_Blocks;
};

inline void GenerateTerrainForChunk(Chunk& chunk, const siv::PerlinNoise& perlin) {
    const int worldOffsetX = chunk.GetCoords().x * CHUNK_SIZE_XZ;
    const int worldOffsetZ = chunk.GetCoords().z * CHUNK_SIZE_XZ;

    for (int x = 0; x < CHUNK_SIZE_XZ; x++) {
        for (int z = 0; z < CHUNK_SIZE_XZ; z++) {
            const int worldX = worldOffsetX + x;
            const int worldZ = worldOffsetZ + z;

            const int height = static_cast<int>(perlin.octave2D_01(worldX * 0.01f, worldZ * 0.01f, 4) * (CHUNK_SIZE_Y - 1));

            for (int y = 0; y <= height - 5; y++) {
                chunk.SetBlockAt(x, y, z, BlockType::Stone);
            }

            for (int y = height - 5; y <= height - 1; y++) {
                chunk.SetBlockAt(x,y,z, BlockType::Dirt);
            }

            chunk.SetBlockAt(x,height, z, BlockType::Grass);

            /* const uint32_t grassChance = Hash2D(worldX, worldZ, 438239);
            const bool spawnGrass = (grassChance % 100) < 3;
            if (spawnGrass) chunk.SetBlockAt(x, height + 1, z, BlockType::GrassPlant);*/


            const uint32_t h = Hash2D(worldX, worldZ, 842492);
            const bool spawnTree = (h % 100) < 1;
            if (spawnTree) {
                chunk.SetBlockAt(x, height + 1, z, BlockType::Log);
                chunk.SetBlockAt(x, height + 2, z, BlockType::Log);
                chunk.SetBlockAt(x, height + 3, z, BlockType::Log);

                CreateBlockPlane(x,z, 5, 5, height + 4, BlockType::Leave, chunk);
                CreateBlockPlane(x,z, 3,3,height+5, BlockType::Leave, chunk);
            }
        }
    }
}

#endif //BLOCKGAME_CHUNK_H
