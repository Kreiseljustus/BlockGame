//
// Created by crisel on 7/20/26.
//

#include "OpenGLBackend.h"

#include "GL/glew.h"

void OpenGLBackend::Begin() {
    glClearColor(255,0,0,255);
    glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLBackend::End() {

}

void OpenGLBackend::Draw(MeshHandle, Material, Transform) {

}

void OpenGLBackend::DrawDynamicBuffer(void *vertexData, int count, PrimitiveType type) {

}

MeshHandle OpenGLBackend::CreateMesh(const MeshData &data) {
    return {0};
}

TextureHandle OpenGLBackend::CreateTexture(const TextureParameters &parameters) {
    return {0};
}

ShaderHandle OpenGLBackend::CreateShader(const ShaderParameters &parameters) {
    return {0};
}

