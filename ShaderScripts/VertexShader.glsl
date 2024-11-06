#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

out vec4 VertexColor;

void main() {
    gl_position = vec4(aPos, 1);
    VertexColor = vec4(aColor, 1);
}