#include "cpu_bitmap.hh"

namespace mge {
CPUBitmap::CPUBitmap(unsigned int width, unsigned int height)
    : m_bitmap(std::unique_ptr<glm::u8vec3[]>(new glm::u8vec3[width * height])),
      m_width(width),
      m_height(height) {}

void CPUBitmap::set(glm::u8vec3 value, unsigned int x, unsigned int y) {
  m_bitmap[y * m_width + x] = value;
}

void CPUBitmap::set(glm::u8vec3 value, const glm::uvec2& p1,
                    const glm::uvec2& p2) {
  unsigned int idx = p1.y * m_width + p1.x;
  for (unsigned int y = p1.y; y < p2.y; ++y) {
    for (unsigned int x = p1.x; x < p2.x; ++x) {
      m_bitmap[idx++] = value;
    }
    idx += (m_width - p2.x + p1.x);
  }
}

glm::u8vec3 CPUBitmap::get(unsigned int x, unsigned int y) const {
  return m_bitmap[y * m_width + x];
}
}  // namespace mge
