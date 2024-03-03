#ifndef MGE_MATH_VEC4_HH
#define MGE_MATH_VEC4_HH

#include "../mgepch.hh"

namespace mge {
template <typename T>
struct vec4 {
  union {
    struct {
      T x, y, z, w;
    };
    struct {
      T r, g, b, a;
    };
  };

  vec4<T>(const T& t) : x(t), y(t), z(t), w(t) {}
  vec4<T>(const T& x, const T& y, const T& z, const T& w)
      : x(x), y(y), z(z), w(w) {}
  vec4<T>(const vec2<T>& v, const T& z, const T& w)
      : x(v.x), y(v.y), z(z), w(w) {}
  vec4<T>(const vec2<T>& v, const vec2<T>& u)
      : x(v.x), y(v.y), z(u.x), w(u.y) {}
  vec4<T>(const vec3<T>& v, const T& w) : x(v.x), y(v.y), z(v.z), w(w) {}

  T& operator[](int idx) {
    switch (idx) {
      case 0:
        return x;
      case 1:
        return y;
      case 2:
        return z;
      case 3:
        return w;
      default:
        throw std::out_of_range();
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
      case 3:
        return w;
      default:
        throw std::out_of_range();
    }
  }
};

template <typename T>
T length(const vec4<T>& v) {
  return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}

template <typename T>
vec4<T> normalize(const vec4<T>& v) {
  T length = length(v);
  return vec4<T>(v.x / length, v.y / length, v.z / length, v.w / length);
}

template <typename T>
T dot(const vec4<T>& v, const vec4<T>& u) {
  return v.x * u.x + v.y * u.y + v.z * u.z + v.w * u.w;
}

template <typename T>
vec4<T> operator+(const vec4<T>& v, const T& a) {
  return vec4<T>(v.x + a, v.y + a, v.z + a, v.w + a);
}

template <typename T>
vec4<T> operator+(const vec4<T>& v, const vec4<T>& u) {
  return vec4<T>(v.x + u.x, v.y + u.y, v.z + u.z, v.w + u.w);
}

template <typename T>
vec4<T> operator-(const vec4<T>& v, const T& a) {
  return vec4<T>(v.x - a, v.y - a, v.z - a, v.w - a);
}

template <typename T>
vec4<T> operator-(const vec4<T>& v, const vec4<T>& u) {
  return vec4<T>(v.x - u.x, v.y - u.y, v.z - u.z, v.w - u.w);
}

template <typename T>
vec4<T> operator*(const vec4<T>& v, const T& a) {
  return vec4<T>(v.x * a, v.y * a, v.z * a, v.w * a);
}

template <typename T>
vec4<T> operator*(const vec4<T>& v, const vec4<T>& u) {
  return dot(v, u);
}

template <typename T>
vec4<T> operator/(const vec4<T>& v, const T& a) {
  return vec4<T>(v.x / a, v.y / a, v.z / a, v.w / a);
}

template <typename T>
vec4<T> operator+=(vec4<T>& v, const T& a) {
  v.x += a;
  v.y += a;
  v.z += a;
  v.w += a;
  return v;
}

template <typename T>
vec4<T> operator+=(vec4<T>& v, const vec4<T>& u) {
  v.x += u.x;
  v.y += u.y;
  v.z += u.z;
  v.w += u.w;
  return v;
}

template <typename T>
vec4<T> operator-=(vec4<T>& v, const T& a) {
  v.x -= a;
  v.y -= a;
  v.z -= a;
  v.w -= a;
  return v;
}

template <typename T>
vec4<T> operator-=(vec4<T>& v, const vec4<T>& u) {
  v.x -= u.x;
  v.y -= u.y;
  v.z -= u.z;
  v.w -= u.w;
  return v;
}

template <typename T>
vec4<T> operator*=(vec4<T>& v, const T& a) {
  v.x *= a;
  v.y *= a;
  v.z *= a;
  v.w *= a;
  return v;
}

template <typename T>
vec4<T> operator*=(vec4<T>& v, const vec4<T>& u) {
  return v = dot(v, u);
}

template <typename T>
vec4<T> operator/=(vec4<T>& v, const T& a) {
  v.x /= a;
  v.y /= a;
  v.z /= a;
  v.w /= a;
  return v;
}
}  // namespace mge

#endif  // MGE_MATH_VEC4_HH