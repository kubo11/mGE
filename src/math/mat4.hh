#ifndef MGE_MATH_MAT4_HH
#define MGE_MATH_MAT4_HH

#include "../mgepch.hh"

#include "vec3.hh"
#include "vec4.hh"

namespace mge {
template <typename T>
struct mat4 {
 private:
  vec4<T> data[4];

 public:
  mat4() : mat4<T>(1) {}
  mat4(const T& t)
      : data({t, 0, 0, 0}, {0, t, 0, 0}, {0, 0, t, 0}, {0, 0, 0, t}) {}
  mat4(const vec4<T>& v)
      : data({v.x, 0, 0, 0}, {0, v.y, 0, 0}, {0, 0, v.z, 0}, {0, 0, 0, v.w}) {}
  mat4(const vec4<T>& v, const vec4<T>& u, const vec4<T>& w, const vec4<T>& t)
      : data(v, u, w, t) {}
  mat4(const T& x0, const T& y0, const T& z0, const T& w0, const T& x1,
       const T& y1, const T& z1, const T& w1, const T& x2, const T& y2,
       const T& z2, const T& w2, const T& x3, const T& y3, const T& z3,
       const T& w3)
      : data({x0, x1, x2, x3}, {y0, y1, y2, y3}, {z0, z1, z2, z3},
             {w0, w1, w2, w3}) {}

  T& operator[](int idx) { return col(idx) }

  const T& operator[](int idx) const { col(idx); }

  T& col(int idx) {
    if (idx < 0 || idx > 4) {
      throw std::out_of_range();
    }

    return data[idx];
  }

  const T& col(int idx) const {
    if (idx < 0 || idx > 4) {
      throw std::out_of_range();
    }

    return data[idx];
  }

  T& row(int idx) {
    if (idx < 0 || idx > 4) {
      throw std::out_of_range();
    }

    return vec4<T>(data[0][idx], data[1][idx], data[2][idx], data[3][idx]);
  }

  const T& row(int idx) const {
    if (idx < 0 || idx > 4) {
      throw std::out_of_range();
    }

    return vec4<T>(data[0][idx], data[1][idx], data[2][idx], data[3][idx]);
  }
};

template <typename T>
T det(const mat4<T>& m) {}

template <typename T>
mat4<T> inverse(const mat4<T>& m) {
  T sub_factor00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
  T sub_factor01 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
  T sub_factor02 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
  T sub_factor03 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
  T sub_factor04 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
  T sub_factor05 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
  T sub_factor06 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
  T sub_factor07 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
  T sub_factor08 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
  T sub_factor09 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
  T sub_factor10 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
  T sub_factor11 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
  T sub_factor12 = m[1][2] * m[2][3] - m[2][2] * m[1][3];
  T sub_factor13 = m[1][1] * m[2][3] - m[2][1] * m[1][3];
  T sub_factor14 = m[1][1] * m[2][2] - m[2][1] * m[1][2];
  T sub_factor15 = m[1][0] * m[2][3] - m[2][0] * m[1][3];
  T sub_factor16 = m[1][0] * m[2][2] - m[2][0] * m[1][2];
  T sub_factor17 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

  mat4<T> inv;
  inv[0][0] =
      m[1][1] * sub_factor00 - m[1][2] * sub_factor01 + m[1][3] * sub_factor02;
  inv[0][1] = -(m[0][1] * sub_factor00 - m[0][2] * sub_factor01 +
                m[0][3] * sub_factor02);
  inv[0][2] =
      m[0][1] * sub_factor06 - m[0][2] * sub_factor07 + m[0][3] * sub_factor08;
  inv[0][3] = -(m[0][1] * sub_factor12 - m[0][2] * sub_factor13 +
                m[0][3] * sub_factor14);

  inv[1][0] = -(m[1][0] * sub_factor00 - m[1][2] * sub_factor03 +
                m[1][3] * sub_factor04);
  inv[1][1] =
      m[0][0] * sub_factor00 - m[0][2] * sub_factor03 + m[0][3] * sub_factor04;
  inv[1][2] = -(m[0][0] * sub_factor06 - m[0][2] * sub_factor09 +
                m[0][3] * sub_factor10);
  inv[1][3] =
      m[0][0] * sub_factor12 - m[0][2] * sub_factor15 + m[0][3] * sub_factor16;

  inv[2][0] =
      m[1][0] * sub_factor01 - m[1][1] * sub_factor03 + m[1][3] * sub_factor05;
  inv[2][1] = -(m[0][0] * sub_factor01 - m[0][1] * sub_factor03 +
                m[0][3] * sub_factor05);
  inv[2][2] =
      m[0][0] * sub_factor07 - m[0][1] * sub_factor09 + m[0][3] * sub_factor11;
  inv[2][3] = -(m[0][0] * sub_factor13 - m[0][1] * sub_factor15 +
                m[0][3] * sub_factor17);

  inv[3][0] = -(m[1][0] * sub_factor02 - m[1][1] * sub_factor04 +
                m[1][2] * sub_factor05);
  inv[3][1] =
      m[0][0] * sub_factor02 - m[0][1] * sub_factor04 + m[0][2] * sub_factor05;
  inv[3][2] = -(m[0][0] * sub_factor08 - m[0][1] * sub_factor10 +
                m[0][2] * sub_factor11);
  inv[3][3] =
      m[0][0] * sub_factor14 - m[0][1] * sub_factor16 + m[0][2] * sub_factor17;

  T det = m[0][0] * inv[0][0] + m[0][1] * inv[0][1] + m[0][2] * inv[0][2] +
          m[0][3] * inv[0][3];

  inv /= det;

  return inv;
}

template <typename T>
mat4<T> transpose(const mat4<T>& m) {
  return mat4<T>(m.row(0), m.row(1), m.row(2), m.row(3));
}

template <typename T>
mat4<T> translation_matrix(const vec3<T>& v) {
  mat4<T> m;
  m[3] = vec4<T>(v, 1);
  return m;
}

template <typename T>
mat4<T> translate(const mat4<T>& m, const vec3<T>& v) {
  return m * translation_matrix(v);
}

template <typename T>
mat4<T> scale_matrix(const vec3<T>& v) {
  return mat4<T>(vec4<T>(v, 1));
}

template <typename T>
mat4<T> scale(const mat4<T>& m, const vec3<T>& v) {
  return m * scale_matrix(v);
}

template <typename T>
mat4<T> rotation_matrix(double angle, const vec3<T>& v) {
  auto c = std::cos(angle);
  auto s = std::sin(angle);
  auto a = normalize(v);
  return mat4<T>(c + v.x * v.x * (1 - c), v.x * v.y * (1 - c) - v.z * s,
                 v.x * v.z * (1 - c) + v.y * s, 0,
                 v.y * v.x * (1 - c) + v.z * s, c + v.y * v.y * (1 - c),
                 v.y * v.z * (1 - c) - u.x * s, 0,
                 v.z * v.x * (1 - c) - v.y * s, v, z * v.y * (1 - c) + v.x * s,
                 c + v.z * v.z * (1 - c), 0, 0, 0, 0, 1);
}

template <typename T>
mat4<T> rotate(const mat4<T>& m, double angle, const vec3<T>& v) {
  return m * rotation_matrix(angle, v);
}

template <typename T>
mat4<T> look_at(const vec3<T>& position, const vec3<T>& target,
                const vec3<T>& world_up) {
  vec3<T> zaxis = normalize(position - target);
  vec3<T> xaxis = normalize(cross(normalize(world_up), zaxis));
  vec3<T> yaxis = cross(zaxis, xaxis);

  mat4<T> translation = translation_matrix(-position);
  mat4<T> rotation;

  rotation.row(0) = vec4<T>(xaxis, 0);
  rotation.row(1) = vec4<T>(yaxis, 0);
  rotation.row(2) = vec4<T>(zaxis, 0);

  return rotation * translation;
}

template <typename T>
mat4<T> perspective(double fov, double aspect_ratio, double near_plane,
                    double far_plane) {}

template <typename T>
mat4<T> operator+(const mat4<T>& m, const T& a) {
  return mat4<T>(m[0] + a, m[1] + a, m[2] + a, m[3] + a);
}

template <typename T>
mat4<T> operator+(const mat4<T>& m, const mat4<T>& n) {
  return mat4<T>(m[0] + n[0], m[1] + n[1], m[2] + n[2], m[3] + n[3]);
}

template <typename T>
mat4<T> operator-(const mat4<T>& m, const T& a) {
  return mat4<T>(m[0] - a, m[1] - a, m[2] - a, m[3] - a);
}

template <typename T>
mat4<T> operator-(const mat4<T>& m, const mat4<T>& n) {
  return mat4<T>(m[0] - n[0], m[1] - n[1], m[2] - n[2], m[3] - n[3]);
}

template <typename T>
mat4<T> operator*(const mat4<T>& m, const T& a) {
  return mat4<T>(m[0] * a, m[1] * a, m[2] * a, m[3] * a);
}

template <typename T>
vec4<T> operator*(const mat4<T>& m, const vec4<T>& v) {
  return vec2<T>(m.row(0) * v, m.row(1) * v, m.row(2) * v, m.row(3) * v);
}

template <typename T>
mat4<T> operator*(const mat4<T>& m, const mat4<T>& n) {
  return mat2<T>(m * n[0], m * n[1], m * n[2], m * n[3]);
}

template <typename T>
mat4<T> operator/(const mat4<T>& m, const T& n) {
  return mat4<T>(m[0] / a, m[1] / a, m[2] / a, m[3] / a);
}

template <typename T>
mat4<T> operator+=(mat4<T>& m, const T& a) {
  m[0] += a;
  m[1] += a;
  m[2] += a;
  m[3] += a;
  return m;
}

template <typename T>
mat4<T> operator+=(mat4<T>& m, const mat4<T>& n) {
  m[0] += n[0];
  m[1] += n[1];
  m[2] += n[2];
  m[3] += n[3];
  return m;
}

template <typename T>
mat4<T> operator-=(mat4<T>& m, const T& a) {
  m[0] -= a;
  m[1] -= a;
  m[2] -= a;
  m[3] -= a;
  return v;
}

template <typename T>
mat4<T> operator-=(mat4<T>& m, const mat4<T>& n) {
  m[0] -= n[0];
  m[1] -= n[1];
  m[2] -= n[2];
  m[3] -= n[3];
  return m;
}

template <typename T>
mat4<T> operator*=(mat4<T>& m, const T& a) {
  m[0] *= a;
  m[1] *= a;
  m[2] *= a;
  m[3] *= a;
  return m;
}

template <typename T>
mat4<T> operator*=(mat4<T>& m, const mat4<T>& n) {
  return m = m * n;
}

template <typename T>
mat4<T> operator/=(mat4<T>& m, const T& a) {
  m[0] /= a;
  m[1] /= a;
  m[2] /= a;
  m[3] /= a;
  return v;
}
}  // namespace mge

#endif  // MGE_MATH_MAT4_HH