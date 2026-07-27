//
// Created by crisel on 7/20/26.
//

#include "OpenGLBackend.h"

#include <iostream>

#include "GL/glew.h"

void OpenGLBackend::Begin() {
    glClearColor(255,0,0,255);
    glClear(GL_COLOR_BUFFER_BIT);

    m_Meshes.reserve(100);
}

void OpenGLBackend::End() {

}

void OpenGLBackend::Draw(MeshHandle mesh, Material material, Transform transform) {
    if (mesh.handle > m_Meshes.size() || mesh.handle < 0) {
        std::cout << "Invalid mesh handle: " << mesh.handle << std::endl;
        return;
    }
    const GPUMesh& gpuMesh = m_Meshes[mesh.handle];

    glUseProgram(material.shaderHandle);

    //TODO: Add support for different texture types
    glBindTexture(TEXTURE_2D, material.textureHandle);

    glBindVertexArray(gpuMesh.vao);
    glDrawElements(GL_TRIANGLES, gpuMesh.indexCount, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void OpenGLBackend::DrawDynamicBuffer(void *vertexData, int count, PrimitiveType type) {

}

MeshHandle OpenGLBackend::CreateMesh(const MeshData &data) {
    GPUMesh mesh;

    glGenVertexArrays(1, &mesh.vao);
    glGenBuffers(1, &mesh.vbo);
    glGenBuffers(1, &mesh.ebo);

    glBindVertexArray(mesh.vao);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, data.indices.size() * sizeof(uint32_t), data.indices.data(), GL_STATIC_DRAW);

    //TODO: Add support for different vertex layouts!!!

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    mesh.indexCount = static_cast<uint32_t>(data.indices.size());

    m_Meshes.push_back(mesh);

    return {static_cast<uint32_t>(m_Meshes.size() - 1)};
}

TextureHandle OpenGLBackend::CreateTexture(const TextureParameters &parameters) {
    return {0};
}

ShaderHandle OpenGLBackend::CreateShader(const ShaderParameters &parameters) {
    return {0};
}

