//
// Created by Kreiseljustus on 7/26/2026.
//

#ifndef BLOCKGAME_RESOURCEMANAGER_H
#define BLOCKGAME_RESOURCEMANAGER_H

#include <unordered_map>

#include "Shader.h"
#include "../headerLibs/json.hpp"
#include "Rendering/RenderData.h"

namespace Engine {
    //TODO: Move shaders into IRenderBackend
    //TODO: Delete methods?
    class ResourceManager {
    public:
        static Rendering::ShaderHandle LoadShader(const std::filesystem::path& vertex, const std::filesystem::path& fragment, std::string alias);
        static Rendering::ShaderHandle GetShader(const std::string& alias);

        static Rendering::MeshData LoadMesh(const std::filesystem::path& meshFile, std::string alias);
        static Rendering::MeshData GetMesh(const std::string& alias);

        static Rendering::TextureParameters LoadTexture(Rendering::TextureParameters& tParams, const std::filesystem::path& texturePath, std::string alias);
        static Rendering::TextureParameters& GetTexture(const std::string& alias);
    private:
        /*//Contains the alias of resources and then the asset bank in which its in (string filepath)
        nlohmann::json assetManifest;
        //Contains the alias of resources + offset, size
        std::vector<nlohmann::json> assetBankManifestos;*/

        static std::unordered_map<std::string, Rendering::Shader> s_Shaders;
        static std::unordered_map<std::string, Rendering::TextureParameters> s_Textures;
        static std::unordered_map<std::string, Rendering::MeshData> s_Meshes;
    };
}

#endif //BLOCKGAME_RESOURCEMANAGER_H
