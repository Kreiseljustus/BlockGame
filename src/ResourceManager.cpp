//
// Created by Kreiseljustus on 7/26/2026.
//

#include "ResourceManager.h"

#include <iostream>

std::unordered_map<std::string, Shader> ResourceManager::s_Shaders;

ShaderHandle ResourceManager::LoadShader(std::filesystem::path vertex, const std::filesystem::path& fragment, const std::string& givenName) {
    Shader s = Shader();
    if (!s.load(vertex,fragment)) {
        std::cout << "Failed to load shader " << vertex << " and " << fragment << std::endl;
        return {0};
    }

    s_Shaders.emplace(givenName, std::move(s));
    return {s.id()};
}

ShaderHandle ResourceManager::GetShader(std::string name) {
    try {
        return {s_Shaders.at(name).id()};
    } catch (const std::out_of_range&) {
        return {0};
    }
}

MeshData ResourceManager::LoadMesh(std::filesystem::path meshFile, std::string alias) {

}

MeshData ResourceManager::GetMesh(std::string alias) {
    try {
        return s_Meshes.at(alias);
    } catch (const std::out_of_range&) {
        //TODO: return cube primitive or something
        return {};
    }
}
