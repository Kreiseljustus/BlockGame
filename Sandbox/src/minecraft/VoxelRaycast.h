//
// Created by Kreiseljustus on 9/26/2026.
//

#ifndef BLOCKGAME_VOXELRAYCAST_H
#define BLOCKGAME_VOXELRAYCAST_H
#include <glm/vec3.hpp>

#include "ChunkManager.h"

struct RaycastResult {
    glm::ivec3 blockPos;
    glm::ivec3 normal;
    bool hit = false;
};

inline RaycastResult VoxelRaycast(glm::vec3 origin, glm::vec3 dir, float maxDist, const ChunkManager& chunks) {
    glm::ivec3 voxel = {
        static_cast<int>(glm::floor(origin.x)),
        static_cast<int>(glm::floor(origin.y)),
        static_cast<int>(glm::floor(origin.z))
    };
    glm::ivec3 step = {dir.x > 0 ? 1 : -1, dir.y > 0 ? 1 : -1, dir.z > 0 ? 1 : -1};

    glm::vec3 tDelta = glm::abs(1.0f / dir);
    glm::vec3 tMax;
    for (int i = 0; i < 3; ++i) {
        float boundary = (step[i] > 0) ? (voxel[i] + 1 - origin[i]) : (origin[i] - voxel[i]);
        tMax[i] = boundary * tDelta[i];
    }

    glm::ivec3 normal{0};

    if (chunks.IsBlockSolid(voxel)) {
        return {voxel, normal, true};
    }

    float traveled = 0;
    while (traveled < maxDist) {
        int axis = (tMax.x < tMax.y) ? (tMax.x < tMax.z ? 0 : 2) : (tMax.y < tMax.z ? 1 : 2);

        voxel[axis] += step[axis];
        traveled = tMax[axis];
        tMax[axis] += tDelta[axis];

        normal = glm::ivec3(0);
        normal[axis] = -step[axis];

        if (chunks.IsBlockSolid(voxel)) {
            return {voxel, normal, true};
        }
    }
    return {};
}

#endif //BLOCKGAME_VOXELRAYCAST_H
