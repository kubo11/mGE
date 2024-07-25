#version 460

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNorm;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

layout(location = 0) out vec3 outPos;
layout(location = 1) out vec3 outNorm;

void main() {
  outPos = (modelMat * vec4(inPos, 1.0)).xyz;
  outNorm = (modelMat * vec4(inNorm, 0.0)).xyz;
  gl_Position = projMat * viewMat * modelMat * vec4(inPos, 1.0);
}