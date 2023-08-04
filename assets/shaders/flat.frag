#version 450
in vec3 vColor;
out vec4 outColor;
uniform vec3 uTint;
void main() {
    outColor = vec4(uTint, 1.0) * vec4(vColor, 1.0);
}