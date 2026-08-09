//
// Created by Kreiseljustus on 8/9/2026.
//

#include "Utils.h"
#include "../minecraft/Chunk.h"

void CreateBlockPlane(int x, int z, int sizeX, int sizeZ, int height, BlockType type, Chunk &chunk) {
    for (int xS = -sizeX / 2; xS <= sizeX / 2; xS++) {
        for (int zS = -sizeZ / 2; zS <= sizeZ / 2; zS++) {
            chunk.SetBlockAt(x + xS, height, z + zS, type);
        }
    }
}
