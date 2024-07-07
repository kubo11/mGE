#ifndef MGE_UTILS
#define MGE_UTILS

#include "mgepch.hh"

namespace mge {
template <class OutType, class InType>
std::unique_ptr<OutType> unique_cast(std::unique_ptr<InType> ptr) {
  return std::unique_ptr<OutType>(dynamic_cast<OutType*>(ptr.release()));
}
}  // namespace mge

#endif  // MGE_UTILS