#version 460

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNorm;

uniform mat4 viewMat;
uniform vec3 color;

out vec4 FragColor;

const vec3 lightPos = vec3(2.0, 2.0, 2.0);
const vec3 lightColor = vec3(1.0, 1.0, 1.0);
const float ka = 0.1, kd = 1.0, ks = 1.5, m = 32.0;

void main() {
  vec3 cameraPos = (inverse(viewMat) * vec4(0.0, 0.0, 0.0, 1.0)).xyz;
  vec3 norm = normalize(inNorm);
  vec3 viewVec = normalize(cameraPos - inPos);
  vec3 lightVec = normalize(lightPos - inPos);
  vec3 halfVec = normalize(viewVec + lightVec);
  vec3 ambient = ka * lightColor;
  vec3 diffuse = kd * color * max(dot(lightVec, norm), 0.0);
  vec3 specular = ks * lightColor * pow(max(dot(halfVec, norm), 0.0), m);
  FragColor = vec4(ambient + diffuse + specular, 1.0);
}