#include "canvas.hh"

namespace mge {
  Canvas::Canvas(glm::ivec2 size, glm::u8vec3 clear_color, bool wrap_x, bool wrap_y) 
    : m_size{size}, m_clear_color{clear_color}, m_wrap_x{wrap_x}, m_wrap_y{wrap_y}, m_data(m_size.x * m_size.y, m_clear_color) {
      auto sd = static_cast<glm::ivec2>(static_cast<glm::vec2>(m_size) * 0.007f);
      m_pen_size = std::max(3, std::max(sd.x, sd.y));
    }

  glm::u8vec3 Canvas::get_pixel(glm::ivec2 pos) const {
    pos = wrap_pos(pos);
    return m_data[pos.y * m_size.x + pos.x];
  }

  glm::u8vec3 Canvas::get_pixel(glm::vec2 pos) const {
    return get_pixel(normalize_pos(pos));
  }

  void Canvas::set_pixel(glm::ivec2 pos, glm::u8vec3 color) {
    pos = wrap_pos(pos);
    m_data[pos.y * m_size.x + pos.x] = color;
  }

  glm::u8vec3 Canvas::get_pen_color() const {
    return m_pen_color;
  }

  void Canvas::set_color(glm::u8vec3 color) {
    m_pen_color = color;
  }

  void Canvas::line(glm::ivec2 start, glm::ivec2 end) {
    // bool flip_xx = false, flip_yy = false, flip_xy = false;
    // glm::ivec2 dest = end - start, offset = start, now = {0, 0};
    // int p = 2 * dest.y - dest.x;

    // if (dest.x < 0) {
    //   dest.x *= -1;
    //   flip_xx = true;
    // }
    // if (dest.y < 0) {
    //   dest.y *= -1;
    //   flip_yy = true;
    // }
    // if (dest.y >= dest.x) {
    //   flip_xy = true;
    // }
  
    // while(now.x <= dest.x) {
    //   auto now1 = now;
    //   if (p <= 0) {
    //       now = {now.x + 1, now.y};
    //       p += 2 * dest.y;
    //   } else {
    //       now = {now.x + 1, now.y + 1};
    //       p += 2 * dest.y - 2 * dest.x;
    //   }
    //   if (flip_xy) {
    //       std::swap(now1.x, now1.y);
    //   }
    //   if (flip_yy) {
    //       now1.y *= -1;
    //   }
    //   if (flip_xx) {
    //       now1.x *= -1;
    //   }
    //   auto point = offset + now1;
    //   int a = m_pen_size/2;
    //   for (int i = -a; i <= a; ++i) {
    //     for (int j = -a; j < a; ++j) {
    //       set_pixel({point.x + i, point.y + j}, m_pen_color);
    //     }
    //   }
    // }

    int x0 = start.x;
    int y0 = start.y;
    int x1 = end.x;
    int y1 = end.y;

    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;

    int err = dx - dy;

    while (true) {
        // Draw the pen
        int a = m_pen_size / 2;
        for (int i = -a; i <= a; ++i) {
            for (int j = -a; j <= a; ++j) {
                set_pixel({x0 + i, y0 + j}, m_pen_color);
            }
        }

        if (x0 == x1 && y0 == y1) break;

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
  }

  void Canvas::line(glm::vec2 start, glm::vec2 end) {
    // line(wrap_pos(normalize_pos(start)), wrap_pos(normalize_pos(end)));
    line(glm::ivec2{m_size.x * start.x, m_size.y * start.y}, glm::ivec2{m_size.x * end.x, m_size.y * end.y});
  }

  void Canvas::flood_fill(glm::ivec2 pos) {
    std::queue<glm::ivec2> pixels;
    pixels.emplace(pos);
    auto color_to_fill = get_pixel(pos);
    while (!pixels.empty()) {
      auto pos = pixels.front();
      pixels.pop();
      if (get_pixel(pos) == color_to_fill) {
        set_pixel(pos, m_pen_color);
        if (pos.x == 0 && m_wrap_x) {
          pixels.emplace(m_size.x - 1, pos.y);
        } else if (pos.x > 0) {
          pixels.emplace(pos.x - 1, pos.y);
        }
        if (pos.x == m_size.x - 1 && m_wrap_x) {
          pixels.emplace(0, pos.y);
        } else if (pos.x < m_size.x - 1) {
          pixels.emplace(pos.x + 1, pos.y);
        }
        if (pos.y == 0 && m_wrap_y) {
          pixels.emplace(pos.x, m_size.y - 1);
        } else if (pos.y > 0) {
          pixels.emplace(pos.x, pos.y - 1);
        }
        if (pos.y == m_size.y - 1 && m_wrap_y) {
          pixels.emplace(pos.x, 0);
        } else if (pos.y < m_size.y - 1) {
          pixels.emplace(pos.x, pos.y + 1);
        }
      }
    }
  }

  void Canvas::flood_fill(glm::vec2 pos) {
    flood_fill(normalize_pos(pos));
  }

  void Canvas::clear() {
    std::fill(m_data.begin(), m_data.end(), m_clear_color);
  }

  const glm::u8vec3* Canvas::get_data() const {
    return m_data.data();
  }

  glm::ivec2 Canvas::wrap_pos(glm::ivec2 pos) const {
    if (m_wrap_x && (pos.x < 0 || pos.x >= m_size.x)) {
      pos.x = pos.x % m_size.x;
      if (pos.x < 0) {
        pos.x += m_size.x;
      }
    }
    if (m_wrap_y && (pos.y < 0 || pos.y >= m_size.y)) {
      pos.y = pos.y % m_size.y;
      if (pos.y < 0) {
        pos.y += m_size.y;
      }
    }

    return pos;
  }

  glm::ivec2 Canvas::normalize_pos(glm::vec2 pos) const {
    return glm::ivec2{(m_size.x - 1) * pos.x, (m_size.y - 1) * pos.y};
  }

  bool Canvas::get_wrap_x() const {
    return m_wrap_x;
  }

  bool Canvas::get_wrap_y() const {
    return m_wrap_y;
  }
} // namespace mge
