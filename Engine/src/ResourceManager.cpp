//
// Created by Kreiseljustus on 7/26/2026.
//

#include "ResourceManager.h"

#include <iostream>

#include "assimp/Importer.hpp"
#include "assimp/mesh.h"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

using namespace Engine;
using namespace Engine::Rendering;

std::unordered_map<std::string, Shader> ResourceManager::s_Shaders;
std::unordered_map<std::string, MeshData> ResourceManager::s_Meshes;

ShaderHandle ResourceManager::LoadShader(std::filesystem::path vertex, const std::filesystem::path& fragment, const std::string& givenName) {
    Shader s = Shader();
    if (!s.load(vertex,fragment)) {
        std::cout << "Failed to load shader " << vertex << " and " << fragment << std::endl;
        return {0};
    }

    const unsigned int id = s.id();
    s_Shaders.emplace(givenName, std::move(s));
    return {id};
}

ShaderHandle ResourceManager::GetShader(std::string name) {
    try {
        return {s_Shaders.at(name).id()};
    } catch (const std::out_of_range&) {
        return {0};
    }
}

MeshData ResourceManager::LoadMesh(std::filesystem::path meshFile, std::string alias) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(meshFile.string(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_GenUVCoords);
    const aiMesh* mesh = scene->mMeshes[0];

    MeshData data;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex v{};
        v.normal = {mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z};
        v.position = {mesh->mVertices[i].x,mesh->mVertices[i].y,mesh->mVertices[i].z};
        v.uv = {0,0};

        data.vertices.push_back(v);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            data.indices.push_back(face.mIndices[j]);
        }
    }

    s_Meshes.emplace(alias, data);
    return data;
}

MeshData ResourceManager::GetMesh(std::string alias) {
    try {
        return s_Meshes.at(alias);
    } catch (const std::out_of_range&) {
        //TODO: return cube primitive or something
        return {};
    }
}
