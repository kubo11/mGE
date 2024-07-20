#version 460 core
layout (location = 0) in vec2 inPos;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec2 inOffset;

layout (location = 0) out vec3 outColor;

void main() {
  outColor = inColor;
  gl_Position = vec4(inPos + inOffset, 0.0, 1.0);
}