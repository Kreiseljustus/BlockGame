#fragment
#version 330 core
out vec4 FragColor;

in vec3 Color;
in vec2 oTex;

uniform sampler2D textureA;

void main() {
    FragColor = texture(textureA, oTex) * vec4(Color, 1.0);
}
