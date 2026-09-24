//
// Created by Kreiseljustus on 7/26/2026.
//

#include "ResourceManager.h"

#include <iostream>
#include <stb_image.h>

#include "assimp/Importer.hpp"
#include "assimp/mesh.h"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

using namespace Engine;
using namespace Engine::Rendering;

std::unordered_map<std::string, Shader> ResourceManager::s_Shaders;
std::unordered_map<std::string, MeshData> ResourceManager::s_Meshes;
std::unordered_map<std::string, TextureParameters> ResourceManager::s_Textures;

ShaderHandle ResourceManager::LoadShader(const std::filesystem::path& vertex, const std::filesystem::path& fragment, std::string alias) {
    Shader s = Shader();
    if (!s.load(vertex,fragment)) {
        std::cout << "Failed to load shader " << vertex << " and " << fragment << std::endl;
        return {0};
    }

    const unsigned int id = s.id();
    s_Shaders.emplace(alias, std::move(s));
    return {id};
}

ShaderHandle ResourceManager::GetShader(const std::string& alias) {
    try {
        return {s_Shaders.at(alias).id()};
    } catch (const std::out_of_range&) {
        return {0};
    }
}

MeshData ResourceManager::LoadMesh(const std::filesystem::path& meshFile, std::string alias) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(meshFile.string(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_GenUVCoords);
    const aiMesh* mesh = scene->mMeshes[0];

    MeshData data;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex v{};
        v.normal = {mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z};
        v.position = {mesh->mVertices[i].x,mesh->mVertices[i].y,mesh->mVertices[i].z};

        if (mesh->mTextureCoords[0]) {
            v.uv = {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};
        } else {
            v.uv = {0,0};
        }

        data.vertices.push_back(v);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            data.indices.push_back(face.mIndices[j]);
        }
    }

    s_Meshes.emplace(alias, data);

    std::cout << "Loaded " << alias << " from " << meshFile << std::endl;

    return data;
}

MeshData ResourceManager::GetMesh(const std::string& alias) {
    try {
        return s_Meshes.at(alias);
    } catch (const std::out_of_range&) {
        //TODO: return cube primitive or something
        return {};
    }
}

TextureParameters ResourceManager::LoadTexture(
    TextureParameters& tParams,
    const std::filesystem::path& texturePath,
    std::string alias)
{
    int channels;

    if (tParams.type != TEXTURE_CUBE_MAP)
    {
        tParams.imageData = stbi_load(
            texturePath.string().c_str(),
            &tParams.width,
            &tParams.height,
            &channels,
            4
        );
    }
    else
    {
        int imageWidth;
        int imageHeight;

        unsigned char* imageData = stbi_load(
            texturePath.string().c_str(),
            &imageWidth,
            &imageHeight,
            &channels,
            4
        );

        if (!imageData)
        {
            std::cerr << "Failed to load cubemap: "
                      << texturePath << std::endl;

            return tParams;
        }

        if (imageWidth % 4 != 0 ||
            imageHeight % 3 != 0 ||
            imageWidth / 4 != imageHeight / 3)
        {
            std::cerr << "Invalid cubemap dimensions: " << imageWidth << "x" << imageHeight << std::endl;

            stbi_image_free(imageData);
            return tParams;
        }

        const int faceSize = imageWidth / 4;
        constexpr int bytesPerPixel = 4;

        tParams.width = faceSize;
        tParams.height = faceSize;

        const size_t faceSizeBytes =
            static_cast<size_t>(faceSize) *
            static_cast<size_t>(faceSize) *
            bytesPerPixel;

        for (auto& face : tParams.cubemapFaces)
        {
            //TODO: Memory leak
            face = new unsigned char[faceSizeBytes];
        }

        auto copyFace =
            [&](unsigned char* dst, const int faceX, const int faceY)
        {
            for (int y = 0; y < faceSize; ++y)
            {
                const unsigned char* src =
                    imageData +
                    (
                        ((faceY * faceSize + y) * imageWidth) +
                        (faceX * faceSize)
                    ) * bytesPerPixel;

                unsigned char* dstRow =
                    dst +
                    y * faceSize * bytesPerPixel;

                std::memcpy(
                    dstRow,
                    src,
                    faceSize * bytesPerPixel
                );
            }
        };

        copyFace(tParams.cubemapFaces[0], 2, 1); // +X
        copyFace(tParams.cubemapFaces[1], 0, 1); // -X
        copyFace(tParams.cubemapFaces[2], 1, 0); // +Y
        copyFace(tParams.cubemapFaces[3], 1, 2); // -Y
        copyFace(tParams.cubemapFaces[4], 1, 1); // +Z
        copyFace(tParams.cubemapFaces[5], 3, 1); // -Z

        stbi_image_free(imageData);
    }

    s_Textures.emplace(alias, tParams);

    std::cout << "Loaded texture " << alias << " from " << texturePath << std::endl;

    return tParams;
}

TextureParameters& ResourceManager::GetTexture(const std::string& alias) {
    try {
        return s_Textures.at(alias);
    } catch (const std::out_of_range&) {
        std::cout << "Failed to load texture :c " << std::endl;
    }
}
