#version 450
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aUV0;

uniform mat4 uModel;

out vec3 vColor;
out vec2 vUV0;

void main() {
    gl_Position = vec4(aPosition, 1.0);
    vColor = aColor;
    vUV0 = aUV0;
}