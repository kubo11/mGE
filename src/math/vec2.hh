#ifndef MGE_MATH_VEC2_HH
#define MGE_MATH_VEC2_HH

#include "../mgepch.hh"

namespace mge {
template <typename T>
struct vec2 {
  union {
    struct {
      T x, y;
    };
    struct {
      T r, a;
    };
  };

  vec2() : x(0), y(0) {}
  vec2(const T& t) : x(t), y(t) {}
  vec2(const T& x, const T& y) : x(x), y(y) {}

  T& operator[](int idx) {
    switch (idx) {
      case 0:
        return x;
      case 1:
        return y;
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
      default:
        throw std::out_of_range();
    }
  }
};

template <typename T>
T length(const vec2<T>& v) {
  return std::sqrt(v.x * v.x + v.y * v.y);
}

template <typename T>
vec2<T> normalize(const vec2<T>& v) {
  T length = length(v);
  return vec2<T>(v.x / length, v.y / length);
}

template <typename T>
T dot(const vec2<T>& v, const vec2<T>& u) {
  return v.x * u.x + v.y * u.y;
}

template <typename T>
vec2<T> operator+(const vec2<T>& v, const T& a) {
  return vec2<T>(v.x + a, v.y + a);
}

template <typename T>
vec2<T> operator+(const vec2<T>& v, const vec2<T>& u) {
  return vec2<T>(v.x + u.x, v.y + u.y);
}

template <typename T>
vec2<T> operator-(const vec2<T>& v, const T& a) {
  return vec2<T>(v.x - a, v.y - a);
}

template <typename T>
vec2<T> operator-(const vec2<T>& v, const vec2<T>& u) {
  return vec2<T>(v.x - u.x, v.y - u.y);
}

template <typename T>
vec2<T> operator*(const vec2<T>& v, const T& a) {
  return vec2<T>(v.x * a, v.y * a);
}

template <typename T>
vec2<T> operator*(const vec2<T>& v, const vec2<T>& u) {
  return dot(v, u);
}

template <typename T>
vec2<T> operator/(const vec2<T>& v, const T& a) {
  return vec2<T>(v.x / a, v.y / a);
}

template <typename T>
vec2<T> operator+=(vec2<T>& v, const T& a) {
  v.x += a;
  v.y += a;
  return v;
}

template <typename T>
vec2<T> operator+=(vec2<T>& v, const vec2<T>& u) {
  v.x += u.x;
  v.y += u.y;
  return v;
}

template <typename T>
vec2<T> operator-=(vec2<T>& v, const T& a) {
  v.x -= a;
  v.y -= a;
  return v;
}

template <typename T>
vec2<T> operator-=(vec2<T>& v, const vec2<T>& u) {
  v.x -= u.x;
  v.y -= u.y;
  return v;
}

template <typename T>
vec2<T> operator*=(vec2<T>& v, const T& a) {
  v.x *= a;
  v.y *= a;
  return v;
}

template <typename T>
vec2<T> operator*=(vec2<T>& v, const vec2<T>& u) {
  return v = dot(v, u);
}

template <typename T>
vec2<T> operator/=(vec2<T>& v, const T& a) {
  v.x /= a;
  v.y /= a;
  return v;
}
}  // namespace mge

#endif  // MGE_MATH_VEC2_HH