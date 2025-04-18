#version 460 core
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inCol;

layout (location = 0) out vec3 outCol;

void main() {
  gl_Position = vec4(inPos, 1.0);
  outCol = inCol;
}