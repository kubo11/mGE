#ifndef MGE_UTILS
#define MGE_UTILS

#include "mgepch.hh"

namespace mge {
template <class OutType, class InType>
inline std::unique_ptr<OutType> unique_cast(std::unique_ptr<InType> ptr) {
  return std::unique_ptr<OutType>(dynamic_cast<OutType*>(ptr.release()));
}

template <class T>
inline void vector_remove(std::vector<T>& vec, const T& obj) {
  vec.erase(std::remove(vec.begin(), vec.end(), obj), vec.end());
}

template <class T>
inline void vector_remove(std::vector<T&>& vec, const T& obj) {
  vec.erase(std::remove(vec.begin(), vec.end(), obj), vec.end());
}

template <class T>
inline void vector_remove(std::vector<std::reference_wrapper<T>>& vec, const T& obj) {
  vec.erase(std::remove(vec.begin(), vec.end(), obj), vec.end());
}

template <class T>
inline void vector_remove_if(std::vector<T>& vec, const std::function<bool(const T&)>& pred) {
  vec.erase(std::remove_if(vec.begin(), vec.end(), pred), vec.end());
}

template <class T>
inline void vector_remove_if(std::vector<T&>& vec, const std::function<bool(const T&)>& pred) {
  vec.erase(std::remove_if(vec.begin(), vec.end(), pred), vec.end());
}

template <class T>
inline void vector_remove_if(std::vector<std::reference_wrapper<T>>& vec,
                             const std::function<bool(const std::reference_wrapper<T>)>& pred) {
  vec.erase(std::remove_if(vec.begin(), vec.end(), pred), vec.end());
}

// type map implementation from
// https://en.cppreference.com/w/cpp/types/type_info/hash_code
using TypeInfoRef = std::reference_wrapper<const std::type_info>;

struct Hasher {
  std::size_t operator()(TypeInfoRef code) const { return code.get().hash_code(); }
};

struct EqualTo {
  bool operator()(TypeInfoRef lhs, TypeInfoRef rhs) const { return lhs.get() == rhs.get(); }
};
}  // namespace mge

#endif  // MGE_UTILS