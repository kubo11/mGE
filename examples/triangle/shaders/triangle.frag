#version 460 core
layout (location = 0) in vec3 inCol;

out vec4 FragColor;

void main() {
  FragColor = vec4(inCol, 1.0f);
}