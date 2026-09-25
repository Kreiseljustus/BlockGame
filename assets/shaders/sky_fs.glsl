#fragment
#version 330 core

in vec3 TexCoords;

out vec4 FragColor;

uniform samplerCube textureA;

void main()
{
    FragColor = texture(textureA, TexCoords);
    }