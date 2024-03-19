#ifndef MGE_MATH_QUAT_HH
#define MGE_MATH_QUAT_HH

#include "../mgepch.hh"

namespace mge {
struct quat {
  float x, y, z, w;

  quat(float s, glm::vec3 v) : x(v.x), y(v.y), z(v.z), w(s) {}
  quat(float w, float x, float y, float z) : x(x), y(y), z(z), w(w) {}
  quat(glm::vec3 eulerAngle) {
    glm::vec3 c = glm::cos(eulerAngle * 0.5f);
    glm::vec3 s = glm::sin(eulerAngle * 0.5f);

    this->w = c.x * c.y * c.z + s.x * s.y * s.z;
    this->x = s.x * c.y * c.z - c.x * s.y * s.z;
    this->y = c.x * s.y * c.z + s.x * c.y * s.z;
    this->z = c.x * c.y * s.z - s.x * s.y * c.z;
  }

  inline quat& operator*=(const quat& r) {
    quat const p(*this);
    quat const q(r);

    this->w = p.w * q.w - p.x * q.x - p.y * q.y - p.z * q.z;
    this->x = p.w * q.x + p.x * q.w + p.y * q.z - p.z * q.y;
    this->y = p.w * q.y + p.y * q.w + p.z * q.x - p.x * q.z;
    this->z = p.w * q.z + p.z * q.w + p.x * q.y - p.y * q.x;
    return *this;
  }
};

inline quat angleAxis(float angle, const glm::vec3& v) {
  float const a(angle);
  float const s = glm::sin(a * 0.5f);

  return {glm::cos(a * 0.5f), v * s};
}

inline float roll(const quat& q) {
  float const y = 2.0f * (q.x * q.y + q.w * q.z);
  float const x = q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z;

  if (glm::all(glm::equal(glm::vec2(x, y), glm::vec2(0.0f, 0.0f),
                          glm::epsilon<float>())))
    return 0.0f;

  return glm::atan(y, x);
}

inline float pitch(const quat& q) {
  float const y = 2.0f * (q.y * q.z + q.w * q.x);
  float const x = q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z;

  if (glm::all(
          glm::equal(glm::vec2(x, y), glm::vec2(0.0f), glm::epsilon<float>())))
    return 2.0f * glm::atan(q.x, q.w);

  return glm::atan(y, x);
}

inline float yaw(const quat& q) {
  return glm::asin(glm::clamp(-2.0f * (q.x * q.z - q.w * q.y), -1.0f, 1.0f));
}

inline glm::vec3 eulerAngles(const quat& q) {
  return glm::vec3(pitch(q), yaw(q), roll(q));
}

inline glm::mat3 mat3_cast(const quat& q) {
  glm::mat3 Result(1.0f);
  float qxx(q.x * q.x);
  float qyy(q.y * q.y);
  float qzz(q.z * q.z);
  float qxz(q.x * q.z);
  float qxy(q.x * q.y);
  float qyz(q.y * q.z);
  float qwx(q.w * q.x);
  float qwy(q.w * q.y);
  float qwz(q.w * q.z);

  Result[0][0] = 1.0f - 2.0f * (qyy + qzz);
  Result[0][1] = 2.0f * (qxy + qwz);
  Result[0][2] = 2.0f * (qxz - qwy);

  Result[1][0] = 2.0f * (qxy - qwz);
  Result[1][1] = 1.0f - 2.0f * (qxx + qzz);
  Result[1][2] = 2.0f * (qyz + qwx);

  Result[2][0] = 2.0f * (qxz + qwy);
  Result[2][1] = 2.0f * (qyz - qwx);
  Result[2][2] = 1.0f - 2.0f * (qxx + qyy);
  return Result;
}

inline glm::mat4 mat4_cast(const quat& q) { return glm::mat4(mat3_cast(q)); }

inline quat operator*(const quat& q1, const quat& q2) { return quat(q1) *= q2; }

inline float dot(const quat& q1, const quat& q2) {
  return q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
}

inline float length(const quat& q) { return glm::sqrt(dot(q, q)); }

inline quat normalize(const quat& q) {
  float len = length(q);
  if (len <= 0.0f) return {1.0f, 0.0f, 0.0f, 0.0f};
  float oneOverLen = 1.0f / len;
  return {q.w * oneOverLen, q.x * oneOverLen, q.y * oneOverLen,
          q.z * oneOverLen};
}

}  // namespace mge

#endif  // MGE_MATH_QUAT_HH