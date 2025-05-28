#ifndef MGE_RENDERER_CANVAS
#define MGE_RENDERER_CANVAS

#include "../mgepch.hh"

namespace mge {
struct Color {
  static constexpr glm::u8vec3 Black = {0, 0, 0};
  static constexpr glm::u8vec3 Blue = {0, 0, 255};
  static constexpr glm::u8vec3 Green = {0, 255, 0};
  static constexpr glm::u8vec3 Red = {255, 0, 0};
  static constexpr glm::u8vec3 White = {255, 255, 255};
};

class Canvas {
 public:
  Canvas(glm::ivec2 size, glm::u8vec3 clear_color = Color::White, bool wrap_x = false, bool wrap_y = false);
  ~Canvas() = default;

  PREVENT_COPY(Canvas);

  glm::u8vec3 get_pixel(glm::ivec2 pos) const;
  glm::u8vec3 get_pixel(glm::vec2 pos) const;
  void set_pixel(glm::ivec2 pos, glm::u8vec3 color);

  glm::u8vec3 get_pen_color() const;
  void set_color(glm::u8vec3 color);

  void line(glm::ivec2 start, glm::ivec2 end);
  void line(glm::vec2 start, glm::vec2 end);
  void flood_fill(glm::ivec2 pos);
  void flood_fill(glm::vec2 pos);
  void clear();

  const glm::u8vec3* get_data() const;

  bool get_wrap_x() const;
  bool get_wrap_y() const;

 private:
  glm::ivec2 m_size;
  glm::u8vec3 m_clear_color;
  std::vector<glm::u8vec3> m_data;
  glm::u8vec3 m_pen_color = Color::Black;
  unsigned int m_pen_size;
  bool m_wrap_x;
  bool m_wrap_y;

  glm::ivec2 wrap_pos(glm::ivec2 pos) const;
  glm::ivec2 normalize_pos(glm::vec2 pos) const;
};

} // namespace mge


#endif // MGE_RENDERER_CANVAS