//
// Created by Kreiseljustus on 7/26/2026.
//

#ifndef BLOCKGAME_RESOURCEMANAGER_H
#define BLOCKGAME_RESOURCEMANAGER_H

#include <unordered_map>

#include "Shader.h"
#include "Rendering/RenderData.h"

namespace Engine {
    //TODO: Move shaders into IRenderBackend
    class ResourceManager {
    public:
        static Rendering::ShaderHandle LoadShader(const std::filesystem::path& vertex, const std::filesystem::path& fragment, const std::string& alias);
        static Rendering::ShaderHandle GetShader(const std::string& alias);

        static Rendering::MeshData LoadMesh(const std::filesystem::path& meshFile, std::string alias);
        static Rendering::MeshData GetMesh(const std::string& alias);
    private:
        static std::unordered_map<std::string, Rendering::Shader> s_Shaders;
        static std::unordered_map<std::string, Rendering::MeshData> s_Meshes;
    };
}

#endif //BLOCKGAME_RESOURCEMANAGER_H
