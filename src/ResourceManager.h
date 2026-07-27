//
// Created by Kreiseljustus on 7/26/2026.
//

#ifndef BLOCKGAME_RESOURCEMANAGER_H
#define BLOCKGAME_RESOURCEMANAGER_H

#include <unordered_map>
#include <unordered_set>

#include "Shader.h"
#include "Rendering/RenderData.h"

class ResourceManager {
public:
    static ShaderHandle LoadShader(std::filesystem::path vertex, const std::filesystem::path& fragment, const std::string& alias);
    static ShaderHandle GetShader(std::string alias);

    static MeshData LoadMesh(std::filesystem::path meshFile, std::string alias);
    static MeshData GetMesh(std::string alias);
private:
    static std::unordered_map<std::string, Shader> s_Shaders;
    static std::unordered_map<std::string, MeshData> s_Meshes;
};


#endif //BLOCKGAME_RESOURCEMANAGER_H
