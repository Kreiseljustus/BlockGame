//
// Created by crisel on 7/20/26.
//

#include "OpenGLBackend.h"

void OpenGLBackend::Begin() {

}

void OpenGLBackend::End() {

}

void OpenGLBackend::Draw(MeshHandle) {
    
}

void OpenGLBackend::DrawDynamicBuffer(void *vertexData, int count, PrimitiveType type) {

}

MeshHandle OpenGLBackend::CreateMesh(const MeshData &data) {
    return {0};
}

TextureHandle OpenGLBackend::CreateTexture() {
    return {0};
}

ShaderHandle OpenGLBackend::CreateShader() {
    return {0};
}
