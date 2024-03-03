#ifndef MGE_MATH_MAT2_HH
#define MGE_MATH_MAT2_HH

#include "../mgepch.hh"

#include "vec2.hh"

namespace mge {
template <typename T>
struct mat2 {
 private:
  vec2<T> data[2];

 public:
  mat2() : mat2<T>(1) {}
  mat2(const T& t) : data({t, 0}, {0, t}) {}
  mat2(const vec2<T>& v) : data({v.x, 0}, {0, v.y}) {}
  mat2(const vec2<T>& v, const vec2<T>& u) : data(v, u) {}
  mat2(const T& x0, const T& y0, const T& x1, const T& y1)
      : data({x0, x1}, {y0, y1}) {}

  T& operator[](int idx) { return col(idx) }

  const T& operator[](int idx) const { col(idx); }

  T& col(int idx) {
    if (idx < 0 || idx > 2) {
      throw std::out_of_range();
    }

    return data[idx];
  }

  const T& col(int idx) const {
    if (idx < 0 || idx > 2) {
      throw std::out_of_range();
    }

    return data[idx];
  }

  T& row(int idx) {
    if (idx < 0 || idx > 2) {
      throw std::out_of_range();
    }

    return vec2<T>(data[0][idx], data[1][idx]);
  }

  const T& row(int idx) const {
    if (idx < 0 || idx > 2) {
      throw std::out_of_range();
    }

    return vec2<T>(data[0][idx], data[1][idx]);
  }
};

template <typename T>
T det(const mat2<T>& m) {
  return dot(m[0], m[1]);
}

template <typename T>
mat2<T> inverse(const mat2<T>& m) {
  return mat2<T>(m[1][1], -m[1][0], -m[0][1], m[0][0]) / det(m[0], m[1]);
}

template <typename T>
mat2<T> transpose(const mat2<T>& m) {
  return mat2<T>(m.row(0), m.row(1));
}

template <typename T>
mat2<T> operator+(const mat2<T>& m, const T& a) {
  return mat2<T>(m[0] + a, m[1] + a);
}

template <typename T>
mat2<T> operator+(const mat2<T>& m, const mat2<T>& n) {
  return mat2<T>(m[0] + n[0], m[1] + n[1]);
}

template <typename T>
mat2<T> operator-(const mat2<T>& m, const T& a) {
  return mat2<T>(m[0] - a, m[1] - a);
}

template <typename T>
mat2<T> operator-(const mat2<T>& m, const mat2<T>& n) {
  return mat2<T>(m[0] - n[0], m[1] - n[1]);
}

template <typename T>
mat2<T> operator*(const mat2<T>& m, const T& a) {
  return mat2<T>(m[0] * a, m[1] * a);
}

template <typename T>
vec2<T> operator*(const mat2<T>& m, const vec2<T>& v) {
  return vec2<T>(m.row(0) * v, m.row(1) * v);
}

template <typename T>
mat2<T> operator*(const mat2<T>& m, const mat2<T>& n) {
  return mat2<T>(m * n[0], m * n[1]);
}

template <typename T>
mat2<T> operator/(const mat2<T>& m, const T& n) {
  return mat2<T>(m[0] / a, m[1] / a);
}

template <typename T>
mat2<T> operator+=(mat2<T>& m, const T& a) {
  m[0] += a;
  m[1] += a;
  return m;
}

template <typename T>
mat2<T> operator+=(mat2<T>& m, const mat2<T>& n) {
  m[0] += n[0];
  m[1] += n[1];
  return m;
}

template <typename T>
mat2<T> operator-=(mat2<T>& m, const T& a) {
  m[0] -= a;
  m[1] -= a;
  return v;
}

template <typename T>
mat2<T> operator-=(mat2<T>& m, const mat2<T>& n) {
  m[0] -= n[0];
  m[1] -= n[1];
  return m;
}

template <typename T>
mat2<T> operator*=(mat2<T>& m, const T& a) {
  m[0] *= a;
  m[1] *= a;
  return m;
}

template <typename T>
mat2<T> operator*=(mat2<T>& m, const mat2<T>& n) {
  return m = m * n;
}

template <typename T>
mat2<T> operator/=(mat2<T>& m, const T& a) {
  m[0] /= a;
  m[1] /= a;
  return v;
}
}  // namespace mge

#endif  // MGE_MATH_MAT2_HH