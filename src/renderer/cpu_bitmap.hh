#ifndef MGE_RENDERER_CPU_BITMAP_HH
#define MGE_RENDERER_CPU_BITMAP_HH

#include "../mgepch.hh"

namespace mge {
class CPUBitmap {
 public:
  CPUBitmap(unsigned int width, unsigned int height);

  void set(glm::u8vec3 value, unsigned int x, unsigned int y);
  void set(glm::u8vec3 value, const glm::uvec2& p1, const glm::uvec2& p2);
  glm::u8vec3 get(unsigned int x, unsigned int y) const;

  inline glm::u8vec3* data() { return m_bitmap.get(); }
  inline const glm::u8vec3* data() const { return m_bitmap.get(); }

  inline const unsigned int get_width() const { return m_width; }
  inline const unsigned int get_height() const { return m_height; }

 private:
  std::unique_ptr<glm::u8vec3[]> m_bitmap;
  unsigned int m_width;
  unsigned int m_height;
};
}  // namespace mge

#endif  // MGE_RENDERER_CPU_BITMAP_HH