#ifndef MGE_MATH_MATRIX_OPERATIONS_HH
#define MGE_MATH_MATRIX_OPERATIONS_HH

#include "../mgepch.hh"

namespace mge {
inline glm::mat4 translate(const glm::vec3& t) {
  auto m = glm::mat4(1.0f);
  m[3] = glm::vec4(t, 1.0f);
  return m;
}
inline glm::mat4 rotate(float angle, glm::vec3 axis) {
  float c = glm::cos(angle);
  float s = glm::sin(angle);

  axis = glm::normalize(axis);
  glm::vec3 temp((1.0f - c) * axis);

  glm::mat4 rotate(1.0f);
  rotate[0][0] = c + temp[0] * axis[0];
  rotate[0][1] = temp[0] * axis[1] + s * axis[2];
  rotate[0][2] = temp[0] * axis[2] - s * axis[1];

  rotate[1][0] = temp[1] * axis[0] - s * axis[2];
  rotate[1][1] = c + temp[1] * axis[1];
  rotate[1][2] = temp[1] * axis[2] + s * axis[0];

  rotate[2][0] = temp[2] * axis[0] + s * axis[1];
  rotate[2][1] = temp[2] * axis[1] - s * axis[0];
  rotate[2][2] = c + temp[2] * axis[2];

  return rotate;
}

inline glm::mat4 rotate(const glm::vec3& a) {
  return rotate(a.y, {0.0f, 1.0f, 0.0f}) * rotate(a.z, {0.0f, 0.0f, 1.0f}) *
         rotate(a.x, {1.0f, 0.0f, 0.0f});
}

inline glm::mat4 scale(const glm::vec3& s) {
  return glm::diagonal4x4(glm::vec4(s, 1.0f));
}
}  // namespace mge

#endif  // MGE_MATH_MATRIX_OPERATIONS_HH