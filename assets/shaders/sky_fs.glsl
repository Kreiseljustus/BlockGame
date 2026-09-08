#fragment
#version 330 core
in vec3 vDirection;
out vec4 FragColor;

void main() {
    float t = clamp(normalize(vDirection).y * 0.5 + 0.5, 0.0, 1.0);
    vec3 color = mix(vec3(0.537,0.569,0.929), vec3(0.745, 0.757, 0.859), t);
    FragColor = vec4(color, 1.0);
}