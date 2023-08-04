#version 450
in vec3 vColor;
in vec2 vUV0;
out vec4 outColor;
uniform vec3 uTint;
uniform sampler2D uDiffuse;
void main() {
    outColor = vec4(vColor, 1.0) * texture(uDiffuse, vUV0);
}