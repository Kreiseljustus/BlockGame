//
// Created by Kreiseljustus on 8/9/2026.
//

#ifndef BLOCKGAME_UTILS_H
#define BLOCKGAME_UTILS_H
#include <cstdint>

class Chunk;
enum class BlockType : uint8_t;

inline uint32_t Hash2D(int x, int z, uint32_t seed) {
    uint32_t h = seed;
    h ^= static_cast<uint32_t>(x) * 0x9E3779B1u;
    h ^= static_cast<uint32_t>(z) * 0x85EBCA77u;
    h ^= h >> 15;
    h *= 0x2C1B3C6Du;
    h ^= h >> 12;
    return h;
}

void CreateBlockPlane(int x, int z, int sizeX, int sizeZ, int height, BlockType type, Chunk& chunk);


#endif //BLOCKGAME_UTILS_H
