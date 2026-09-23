#vertex
#version 330 core

layout(location = 0) in vec3 aPos;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

out vec3 vDirection;

void main()
{
    // Remove camera translation so the sky follows the camera.
    mat4 viewNoTranslation = mat4(mat3(view));

    vec4 pos = projection *
               viewNoTranslation *
               model *
               vec4(aPos, 1.0);

    gl_Position = pos.xyww;

    vDirection = aPos;
}