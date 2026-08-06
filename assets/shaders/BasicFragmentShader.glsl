#fragment
#version 330 core
out vec4 FragColor;

in vec2 oTex;
in vec3 vNormals;

uniform sampler2D textureA;

void main() {
    vec3 normal = normalize(vNormals);
    vec3 lightDir = normalize(vec3(0.5, 1.0, 0.3));
    float diffuse = max(dot(normal, lightDir), 0.0);
    float ambient = 0.15;

    vec3 texColor = texture(textureA, oTex).rgb;
    FragColor = vec4(texColor * (ambient + diffuse), 1.0);
}
