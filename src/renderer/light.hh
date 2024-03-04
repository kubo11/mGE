#ifndef MGE_RENDERER_LIGHT_HH
#define MGE_RENDERER_LIGHT_HH

#include "../mgepch.hh"

#include "../math/vec3.hh"

namespace mge {
class Light {
 public:
  Light();
  virtual ~Light() {}

 private:
  vec3<uint8_t> m_color;
  vec3<float> m_position;
};
}  // namespace mge

#endif  // MGE_RENDERER_LIGHT_HH