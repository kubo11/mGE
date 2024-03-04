#ifndef MGE_MATH_VEC3_HH
#define MGE_MATH_VEC3_HH

#include "../mgepch.hh"

#include "vec2.hh"

namespace mge {
template <typename T>
struct vec3 {
  union {
    struct {
      T x, y, z;
    };
    struct {
      T r, g, b;
    };
  };

  vec3() : x(0), y(0), z(0) {}
  vec3(const T& t) : x(t), y(t), z(t) {}
  vec3(const T& x, const T& y, const T& z) : x(x), y(y), z(z) {}
  vec3(const vec2<T>& v, const T& z) : x(v.x), y(v.y), z(z) {}

  T& operator[](int idx) {
    switch (idx) {
      case 0:
        return x;
      case 1:
        return y;
      case 2:
        return z;
      default:
        throw std::out_of_range("idx in [0, 2]");
    }
  }

  const T& operator[](int idx) const {
    switch (idx) {
      case 0:
        return x;
      case 1:
        return y;
      case 2:
        return z;
      default:
        throw std::out_of_range("idx in [0, 2]");
    }
  }
};

template <typename T>
T length(const vec3<T>& v) {
  return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

template <typename T>
vec3<T> normalize(const vec3<T>& v) {
  T len = length(v);
  return vec3<T>(v.x / len, v.y / len, v.z / len);
}

template <typename T>
T dot(const vec3<T>& v, const vec3<T>& u) {
  return v.x * u.x + v.y * u.y + v.z * u.z;
}

template <typename T>
vec3<T> cross(const vec3<T>& v, const vec3<T>& u) {
  return vec3<T>(v.y * u.z - v.z * u.y, v.z * u.x - v.x * u.z,
                 v.x * u.y - u.y * v.x);
}

template <typename T>
vec3<T> operator+(const vec3<T>& v, const T& a) {
  return vec3<T>(v.x + a, v.y + a, v.z + a);
}

template <typename T>
vec3<T> operator+(const vec3<T>& v, const vec3<T>& u) {
  return vec3<T>(v.x + u.x, v.y + u.y, v.z + u.z);
}

template <typename T>
vec3<T> operator-(const vec3<T>& v, const T& a) {
  return vec3<T>(v.x - a, v.y - a, v.z - a);
}

template <typename T>
vec3<T> operator-(const vec3<T>& v, const vec3<T>& u) {
  return vec3<T>(v.x - u.x, v.y - u.y, v.z - u.z);
}

template <typename T>
vec3<T> operator*(const T& a, const vec3<T>& v) {
  return vec3<T>(v.x * a, v.y * a, v.z * a);
}

template <typename T>
vec3<T> operator*(const vec3<T>& v, const T& a) {
  return vec3<T>(v.x * a, v.y * a, v.z * a);
}

template <typename T>
T operator*(const vec3<T>& v, const vec3<T>& u) {
  return dot(v, u);
}

template <typename T>
vec3<T> operator/(const vec3<T>& v, const T& a) {
  return vec3<T>(v.x / a, v.y / a, v.z / a);
}

template <typename T>
vec3<T> operator+=(vec3<T>& v, const T& a) {
  v.x += a;
  v.y += a;
  v.z += a;
  return v;
}

template <typename T>
vec3<T> operator+=(vec3<T>& v, const vec3<T>& u) {
  v.x += u.x;
  v.y += u.y;
  v.z += u.z;
  return v;
}

template <typename T>
vec3<T> operator-=(vec3<T>& v, const T& a) {
  v.x -= a;
  v.y -= a;
  v.z -= a;
  return v;
}

template <typename T>
vec3<T> operator-=(vec3<T>& v, const vec3<T>& u) {
  v.x -= u.x;
  v.y -= u.y;
  v.z -= u.z;
  return v;
}

template <typename T>
vec3<T> operator*=(vec3<T>& v, const T& a) {
  v.x *= a;
  v.y *= a;
  v.z *= a;
  return v;
}

template <typename T>
vec3<T> operator*=(vec3<T>& v, const vec3<T>& u) {
  return v = dot(v, u);
}

template <typename T>
vec3<T> operator/=(vec3<T>& v, const T& a) {
  v.x /= a;
  v.y /= a;
  v.z /= a;
  return v;
}
}  // namespace mge

#endif  // MGE_MATH_VEC3_HH