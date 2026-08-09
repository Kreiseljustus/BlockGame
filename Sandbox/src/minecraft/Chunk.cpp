//
// Created by Kreiseljustus on 8/9/2026.
//
#include "Chunk.h"

#include "Rendering/RenderData.h"
#include <iostream>

using namespace Engine::Rendering;

MeshData Chunk::GenerateChunkMesh(const Chunk& chunk) {
    MeshData data;

    auto addFace = [&](const glm::vec3 blockPos, const glm::vec3 normal, const glm::vec3 corners[4]) {
        const auto start = static_cast<uint32_t>(data.vertices.size());
        glm::vec2 uvs[4] = {{0,0}, {1,0}, {1,1}, {0,1}};

        for (int i = 0; i < 4; i++) {
            Vertex v;
            v.position = blockPos + corners[i];
            v.normal = normal;
            v.uv = uvs[i];
            data.vertices.push_back(v);
        }


        data.indices.push_back(start);
        data.indices.push_back(start + 1);
        data.indices.push_back(start + 2);
        data.indices.push_back(start);
        data.indices.push_back(start + 2);
        data.indices.push_back(start + 3);
    };

    for (int x = 0; x < CHUNK_SIZE_XZ; x++) {
        for (int y = 0; y < CHUNK_SIZE_Y; y++) {
            for (int z = 0; z < CHUNK_SIZE_XZ; z++) {
                if (chunk.GetBlockAt(x,y,z) == BlockType::Air) continue;

                const glm::vec3 pos = {static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)};

                //+X face
                if (chunk.GetBlockAt(x + 1, y, z) == BlockType::Air) {
                    const glm::vec3 c[4] = {{1,0,0}, {1,1,0}, {1,1,1}, {1,0,1}};
                    addFace(pos, {1, 0, 0}, c);
                }
                //-X face
                if (chunk.GetBlockAt(x - 1, y, z) == BlockType::Air) {
                    const glm::vec3 c[4] = {{0,0,1}, {0,1,1}, {0,1,0}, {0,0,0}};
                    addFace(pos, {-1, 0, 0}, c);
                }
                //+Y face (top)
                if (chunk.GetBlockAt(x, y + 1, z) == BlockType::Air) {
                    const glm::vec3 c[4] = {{0,1,0}, {0,1,1}, {1,1,1}, {1,1,0}};
                    addFace(pos, {0, 1, 0}, c);
                }
                //-Y face (bottom)
                if (chunk.GetBlockAt(x, y - 1, z) == BlockType::Air) {
                    const glm::vec3 c[4] = {{0,0,1}, {0,0,0}, {1,0,0}, {1,0,1}};
                    addFace(pos, {0, -1, 0}, c);
                }
                //+Z face
                if (chunk.GetBlockAt(x, y, z + 1) == BlockType::Air) {
                    const glm::vec3 c[4] = {{0,0,1}, {1,0,1}, {1,1,1}, {0,1,1}};
                    addFace(pos, {0, 0, 1}, c);
                }

                if (chunk.GetBlockAt(x, y, z - 1) == BlockType::Air) {
                    const glm::vec3 c[4] = {{1,0,0}, {0,0,0}, {0,1,0}, {1,1,0}};
                    addFace(pos, {0, 0, -1}, c);
                }
            }
        }
    }
    return data;
}


BlockType Chunk::GetBlockAt(const int x, const int y, const int z) const {
    if (x < 0 || x >= CHUNK_SIZE_XZ || y < 0 || y >= CHUNK_SIZE_Y || z < 0 || z >= CHUNK_SIZE_XZ)
        return BlockType::Air;
    return m_Blocks[Get3DTO1DBlockIndex(x, y, z)];
}

void Chunk::SetBlockAt(const int x, const int y, const int z, const BlockType type) {
    if (x < 0 || x >= CHUNK_SIZE_XZ || y < 0 || y >= CHUNK_SIZE_Y || z < 0 || z >= CHUNK_SIZE_XZ) {
        std::cout << "SetBlockAt out of bounds: " << x << "," << y << "," << z << std::endl;
        return;
    }
    m_Blocks[Get3DTO1DBlockIndex(x, y, z)] = type;
}

int Chunk::Get3DTO1DBlockIndex(const int x, const int y, const int z){
    return x + y * CHUNK_SIZE_XZ + z * CHUNK_SIZE_XZ * CHUNK_SIZE_Y;
}
