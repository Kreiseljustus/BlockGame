//
// Created by crisel on 7/20/26.
//

#include "OpenGLBackend.h"

#include <iostream>

#include "GL/glew.h"
#include "glm/fwd.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

using namespace Engine::Rendering;

void OpenGLBackend::Begin() {
    glClearColor(0.5,0.5,0.5,255);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_Meshes.reserve(100);
}

void OpenGLBackend::End() {

}

void OpenGLBackend::Draw(const MeshHandle mesh, const Material material, const Transform transform) {
    if (mesh.handle > m_Meshes.size() || mesh.handle < 0) {
        std::cout << "Invalid mesh handle: " << mesh.handle << std::endl;
        return;
    }
    const GPUMesh& gpuMesh = m_Meshes[mesh.handle];

    glUseProgram(material.shaderHandle);

    //Position -> Rotation -> Scale

    glm::mat4 model = glm::translate(glm::mat4(1.0f), transform.position);
    model = glm::rotate(model, glm::radians(transform.rotation.x), glm::vec3(1,0,0));
    model = glm::rotate(model, glm::radians(transform.rotation.y), glm::vec3(0,1,0));
    model = glm::rotate(model, glm::radians(transform.rotation.z), glm::vec3(0,0,1));

    model = glm::scale(model, transform.scale);

    glm::mat4 mvp = m_Proj * m_View * model;

    const GLint mvpLoc = glGetUniformLocation(material.shaderHandle, "transform");

    if (mvpLoc == -1) {
        std::cout << "Uniform not found!" << " transform" << std::endl;
    }

    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, &mvp[0][0]);

    //TODO: Add support for different texture types
    if (material.textureHandle != -1) {
        const GPUTexture& tex = m_Textures[material.textureHandle];
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(TEXTURE_2D, tex.id);

        const GLint texLoc = glGetUniformLocation(material.shaderHandle, "textureA");
        glUniform1i(texLoc, 0);
    }

    glBindVertexArray(gpuMesh.vao);
    glDrawElements(GL_TRIANGLES, static_cast<int>(gpuMesh.indexCount), GL_UNSIGNED_INT, nullptr);
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
    glBufferData(GL_ARRAY_BUFFER, data.vertices.size() * sizeof(Vertex), data.vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.indices.size() * sizeof(uint32_t), data.indices.data(), GL_STATIC_DRAW);

    //TODO: Add support for different vertex layouts!!!

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, position)));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, normal)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, uv)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    mesh.indexCount = static_cast<uint32_t>(data.indices.size());

    m_Meshes.push_back(mesh);

    return {static_cast<uint32_t>(m_Meshes.size() - 1)};
}

TextureHandle OpenGLBackend::CreateTexture(const TextureParameters& parameters) {
    GPUTexture tex;
    tex.width = parameters.width;
    tex.height = parameters.height;
    tex.format = static_cast<GLenum>(parameters.format);

    glGenTextures(1, &tex.id);
    glBindTexture(GL_TEXTURE_2D, tex.id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, parameters.wrapping);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, parameters.wrapping);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, parameters.filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, parameters.filter);

    glTexImage2D(GL_TEXTURE_2D, 0, static_cast<int>(tex.format), tex.width, tex.height, 0,
             tex.format, parameters.dataType, parameters.imageData);

    glBindTexture(GL_TEXTURE_2D, 0);

    m_Textures.push_back(tex);
    return {static_cast<uint32_t>(m_Textures.size() - 1)};
}

ShaderHandle OpenGLBackend::CreateShader(const ShaderParameters &parameters) {
    return {0};
}

void OpenGLBackend::UpdateTexture(const TextureHandle handle, const void *pixelData) {
    if (handle.handle >= m_Textures.size()) {
        std::cout << "Invalid texture handle: " << handle.handle << std::endl;
        return;
    }

    //TODO: Add texture types

    const GPUTexture& tex = m_Textures[handle.handle];
    glBindTexture(GL_TEXTURE_2D, tex.id);

    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, tex.width, tex.height, tex.format, GL_UNSIGNED_BYTE, pixelData);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void OpenGLBackend::SetViewProjection(const glm::mat4 &view, const glm::mat4 &proj) {
    m_View = view;
    m_Proj = proj;
}

