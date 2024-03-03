#ifndef MGE_MATH_MAT3_HH
#define MGE_MATH_MAT3_HH

#include "../mgepch.hh"

#include "vec3.hh"

namespace mge {
template <typename T>
struct mat3 {
 private:
  vec3<T> data[3];

 public:
  mat3<T>() : mat3<T>(1) {}
  mat3<T>(const T& t) : data({t, 0, 0}, {0, t, 0}, {0, 0, t}) {}
  mat3<T>(const vec3<T>& v) : data({v.x, 0, 0}, {0, v.y, 0}, {0, 0, v.z}) {}
  mat3<T>(const vec3<T>& v, const vec3<T>& u, const vec3<T>& w)
      : data(v, u, w) {}
  mat3<T>(const T& x0, const T& y0, const T& z0, const T& x1, const T& y1,
          const T& z1, const T& x2, const T& y2, const T& z2)
      : data({x0, x1, x2}, {y0, y1, y2}, {z0, z1, z2}) {}

  T& operator[](int idx) { return col(idx) }

  const T& operator[](int idx) const { col(idx); }

  T& col(int idx) {
    if (idx < 0 || idx > 3) {
      throw std::out_of_range();
    }

    return data[idx];
  }

  const T& col(int idx) const {
    if (idx < 0 || idx > 3) {
      throw std::out_of_range();
    }

    return data[idx];
  }

  T& row(int idx) {
    if (idx < 0 || idx > 3) {
      throw std::out_of_range();
    }

    return vec3<T>(data[0][idx], data[1][idx], data[2][idx]);
  }

  const T& row(int idx) const {
    if (idx < 0 || idx > 3) {
      throw std::out_of_range();
    }

    return vec3<T>(data[0][idx], data[1][idx], data[2][idx]);
  }
};

template <typename T>
T det(const mat3<T>& m) {
  return m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2]) -
         m[1][0] * (m[0][1] * m[2][2] - m[2][1] * m[0][2]) +
         m[2][0] * (m[0][1] * m[1][2] - m[1][1] * m[0][2]);
}

template <typename T>
mat3<T> inverse(const mat3<T>& m) {
  mat3<T> inv;
  inv[0][0] = m[1][1] * m[2][2] - m[2][1] * m[1][2];
  inv[0][1] = -(m[0][1] * m[2][2] - m[2][1] * m[0][2]);
  inv[0][2] = m[0][1] * m[1][2] - m[1][1] * m[0][2];
  inv[1][0] = -(m[1][0] * m[2][2] - m[2][0] * m[1][2]);
  inv[1][1] = m[0][0] * m[2][2] - m[2][0] * m[0][2];
  inv[1][2] = -(m[0][0] * m[1][2] - m[1][0] * m[0][2]);
  inv[2][0] = m[1][0] * m[2][1] - m[2][0] * m[1][1];
  inv[2][1] = -(m[0][0] * m[2][1] - m[2][0] * m[0][1]);
  inv[2][2] = m[0][0] * m[1][1] - m[1][0] * m[0][1];
  inv /= det(m);

  return inv;
}

template <typename T>
mat3<T> transpose(const mat3<T>& m) {
  return mat3<T>(m.row(0), m.row(1), m.row(2));
}

template <typename T>
mat3<T> operator+(const mat3<T>& m, const T& a) {
  return mat3<T>(m[0] + a, m[1] + a, m[2] + a);
}

template <typename T>
mat3<T> operator+(const mat3<T>& m, const mat3<T>& n) {
  return mat3<T>(m[0] + n[0], m[1] + n[1], m[2] + n[2]);
}

template <typename T>
mat3<T> operator-(const mat3<T>& m, const T& a) {
  return mat3<T>(m[0] - a, m[1] - a, m[2] - a);
}

template <typename T>
mat3<T> operator-(const mat3<T>& m, const mat3<T>& n) {
  return mat3<T>(m[0] - n[0], m[1] - n[1], m[2] - n[2]);
}

template <typename T>
mat3<T> operator*(const mat3<T>& m, const T& a) {
  return mat3<T>(m[0] * a, m[1] * a, m[2] * a);
}

template <typename T>
vec3<T> operator*(const mat3<T>& m, const vec3<T>& v) {
  return vec2<T>(m.row(0) * v, m.row(1) * v, m.row(2) * v);
}

template <typename T>
mat3<T> operator*(const mat3<T>& m, const mat3<T>& n) {
  return mat2<T>(m * n[0], m * n[1], m * n[2]);
}

template <typename T>
mat3<T> operator/(const mat3<T>& m, const T& n) {
  return mat3<T>(m[0] / a, m[1] / a, m[2] / a);
}

template <typename T>
mat3<T> operator+=(mat3<T>& m, const T& a) {
  m[0] += a;
  m[1] += a;
  m[2] += a;
  return m;
}

template <typename T>
mat3<T> operator+=(mat3<T>& m, const mat3<T>& n) {
  m[0] += n[0];
  m[1] += n[1];
  m[2] += n[2];
  return m;
}

template <typename T>
mat3<T> operator-=(mat3<T>& m, const T& a) {
  m[0] -= a;
  m[1] -= a;
  m[2] -= a;
  return v;
}

template <typename T>
mat3<T> operator-=(mat3<T>& m, const mat3<T>& n) {
  m[0] -= n[0];
  m[1] -= n[1];
  m[2] -= n[2];
  return m;
}

template <typename T>
mat3<T> operator*=(mat3<T>& m, const T& a) {
  m[0] *= a;
  m[1] *= a;
  m[2] *= a;
  return m;
}

template <typename T>
mat3<T> operator*=(mat3<T>& m, const mat3<T>& n) {
  return m = m * n;
}

template <typename T>
mat3<T> operator/=(mat3<T>& m, const T& a) {
  m[0] /= a;
  m[1] /= a;
  m[2] /= a;
  return v;
}
}  // namespace mge

#endif  // MGE_MATH_MAT3_HH