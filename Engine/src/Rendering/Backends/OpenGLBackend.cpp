//
// Created by crisel on 7/20/26.
//

#include "OpenGLBackend.h"

#include <iostream>

#include <glm/gtc/type_ptr.hpp>
#include "GL/glew.h"
#include "glm/fwd.hpp"
#include "glm/ext/matrix_transform.hpp"

using namespace Engine::Rendering;

OpenGLBackend::OpenGLBackend() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glFrontFace(GL_CCW);

    m_Textures.push_back({0});
}

void OpenGLBackend::Begin() {
    glClearColor(0.5,0.5,0.5,255);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_ShadersUpdatedCurrentFrame.clear();
}

void OpenGLBackend::End() {

}

void OpenGLBackend::Draw(const MeshHandle mesh, const Material material, const Transform transform) {
    if (mesh.backendID >= m_Meshes.size() || !m_Meshes[mesh.backendID].alive || m_Meshes[mesh.backendID].generation != mesh.generation) {
        std::cout << "Invalid or stale mesh handle" << std::endl;
        return;
    }
    const GPUMesh& gpuMesh = m_Meshes[mesh.backendID];

    glUseProgram(material.shaderHandle.handle);

    uint32_t shaderHandle = material.shaderHandle.handle;

    if (m_ShadersUpdatedCurrentFrame.find(material.shaderHandle.handle) == std::end(m_ShadersUpdatedCurrentFrame)) {
        GLint viewLoc = glGetUniformLocation(shaderHandle, "view");
        GLint projLoc = glGetUniformLocation(shaderHandle, "projection");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(m_View));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(m_Proj));
        m_ShadersUpdatedCurrentFrame.insert(shaderHandle);
    }

    //Position -> Rotation -> Scale

    glm::mat4 model = glm::translate(glm::mat4(1.0f), transform.position);
    model = glm::rotate(model, glm::radians(transform.rotation.x), glm::vec3(1,0,0));
    model = glm::rotate(model, glm::radians(transform.rotation.y), glm::vec3(0,1,0));
    model = glm::rotate(model, glm::radians(transform.rotation.z), glm::vec3(0,0,1));

    model = glm::scale(model, transform.scale);

    const GLint modelLoc = glGetUniformLocation(material.shaderHandle.handle, "model");

    if (modelLoc == -1) {
        std::cout << "Uniform not found!" << " model" << std::endl;
    }

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glBindTexture(GL_TEXTURE_2D, 0);

    //TODO: Add support for different texture types
    if (material.textureHandle.handle != 0) {
        if (material.textureHandle.handle >= m_Textures.size()) {
            std::cout << "Invalid texture handle: " << material.textureHandle.handle << std::endl;
        } else {
            const GPUTexture& tex = m_Textures[material.textureHandle.handle];
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(TEXTURE_2D, tex.id);

            const GLint texLoc = glGetUniformLocation(material.shaderHandle.handle, "textureA");
            glUniform1i(texLoc, 0);
        }
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
    mesh.alive = true;

    uint32_t index;
    if (!m_FreeMeshSlots.empty()) {
        index = m_FreeMeshSlots.back();
        m_FreeMeshSlots.pop_back();
        mesh.generation = m_Meshes[index].generation + 1;
        m_Meshes[index] = mesh;
    } else {
        index = static_cast<uint32_t>(m_Meshes.size());
        mesh.generation = 0;
        m_Meshes.push_back(mesh);
    }

    return {index, mesh.generation};
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

void OpenGLBackend::UpdateMesh(MeshHandle handle, const MeshData &data) {
    if (handle.backendID >= m_Meshes.size() || !m_Meshes[handle.backendID].alive) {
        std::cerr << "Invalid mesh handle in UpdateMesh: " << handle.backendID << std::endl;
        return;
    }
    if (m_Meshes[handle.backendID].generation != handle.generation) {
        std::cerr << "Stale mesh handle in UpdateMesh: " << handle.backendID << std::endl;
        return;
    }

    GPUMesh& mesh = m_Meshes[handle.backendID];

    glBindVertexArray(mesh.vao);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, data.vertices.size() * sizeof(Vertex), data.vertices.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.indices.size() * sizeof(uint32_t), data.indices.data(), GL_DYNAMIC_DRAW);

    glBindVertexArray(0);

    mesh.indexCount = static_cast<uint32_t>(data.indices.size());
}

void OpenGLBackend::DestroyMesh(MeshHandle handle) {
    if (handle.backendID >= m_Meshes.size() || !m_Meshes[handle.backendID].alive) {
        std::cerr << "Mesh handle already deleted!" << std::endl;
        return;
    }
    if (m_Meshes[handle.backendID].generation != handle.generation) {
        std::cerr << "Stale mesh handle!" << std::endl;
        return;
    }

    GPUMesh& mesh = m_Meshes[handle.backendID];
    glDeleteVertexArrays(1, &mesh.vao);
    glDeleteBuffers(1, &mesh.vbo);
    glDeleteBuffers(1, &mesh.ebo);
    mesh.alive = false;

    m_FreeMeshSlots.push_back(handle.backendID);
}

void OpenGLBackend::SetViewProjection(const glm::mat4 &view, const glm::mat4 &proj) {
    m_View = view;
    m_Proj = proj;

    //TODO: SHADER UPLOAD STUFF
}

