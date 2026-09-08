#vertex
#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 u_View;
uniform mat4 u_Proj;

out vec3 vDirection;

void main() {
    mat4 viewNoTranslation = mat4(mat3(u_View));

    vec4 pos = u_Proj * viewNoTranslation * vec4(aPos, 1.0);
    gl_Position = pos.xyww;

    vDirection = aPos;
}