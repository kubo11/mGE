#include "adaptive_raycaster.hh"

namespace mge {
RaycastingTarget::RaycastingTarget(Material material) : m_material(material) {}

AdaptiveRaycaster::AdaptiveRaycaster(uint8_t max_pixel_size_exponent,
                                     unsigned int width, unsigned int height,
                                     glm::u8vec3 background_color)
    : m_bitmap(width, height),
      m_canvas(),
      m_max_pixel_size_exponent(max_pixel_size_exponent),
      m_pixel_size(get_max_pixel_size()),
      m_width(width),
      m_height(height),
      m_background_color(background_color) {
  m_canvas.init(m_bitmap);
}

void AdaptiveRaycaster::reset() { m_pixel_size = get_max_pixel_size(); }

void AdaptiveRaycaster::draw(const RaycastingTarget& target,
                             const Camera& camera) {
  if (m_pixel_size > 0) {
    MGE_INFO("Adaptive raycaster update with pixel size {}", m_pixel_size);
    update(target, camera);
    m_pixel_size >>= 1;
  }

  m_canvas.draw();
}

void AdaptiveRaycaster::update(const RaycastingTarget& target,
                               const Camera& camera) {
  glm::mat4 camera_matrix = camera.get_projection_view_matrix();
  glm::mat4 target_matrix =
      glm::transpose(camera_matrix) * target.get_matrix() * camera_matrix;

  const int half_pixel_size = m_pixel_size >> 1;
  bool is_row_even = true;
  for (int center_y = 0; center_y < m_height + half_pixel_size;
       center_y += m_pixel_size, is_row_even = !is_row_even) {
    int start_x, delta_x;
    if (m_pixel_size != get_max_pixel_size() && is_row_even) {
      start_x = m_pixel_size;
      delta_x = 2 * m_pixel_size;
    } else {
      start_x = 0;
      delta_x = m_pixel_size;
    }

    for (int center_x = start_x; center_x < m_width + half_pixel_size;
         center_x += delta_x) {
      glm::u8vec3 color = compute_color(
          target_matrix, target, camera,
          glm::vec2(2 * center_x / m_width - 1, 2 * center_y / m_height - 1));

      auto p1 = glm::uvec2(std::max(center_x - half_pixel_size, 0),
                           std::max(center_y - half_pixel_size, 0));
      auto p2 = glm::uvec2(
          std::min(center_x + (half_pixel_size != 0 ? half_pixel_size : 1),
                   static_cast<int>(m_width)),
          std::min(center_y + (half_pixel_size != 0 ? half_pixel_size : 1),
                   static_cast<int>(m_height)));
      m_bitmap.set(color, p1, p2);
    }
  }

  m_canvas.update(m_bitmap);
}

uint8_t AdaptiveRaycaster::get_max_pixel_size() const {
  return 1 << m_max_pixel_size_exponent;
}

std::optional<float> AdaptiveRaycaster::compute_intersection(
    const glm::mat4& matrix, const glm::vec2& pos) {
  float a = matrix[2][2];
  float b = (matrix[2][0] + matrix[0][2]) * pos.x +
            (matrix[2][1] + matrix[1][2]) * pos.y + matrix[2][3] + matrix[3][2];
  float c = (matrix[0][0] * pos.x + matrix[0][1] * pos.y + matrix[0][3] +
             matrix[3][0]) *
                pos.x +
            (matrix[1][0] * pos.x + matrix[1][1] * pos.y + matrix[1][3] +
             matrix[3][1]) *
                pos.y +
            matrix[3][3];

  float delta = b * b - 4 * a * c;

  if (delta <= 0) {
    return std::nullopt;
  }

  float z = (-b - std::sqrt(delta)) / (2 * a);

  if (z < -1 || z > 1) {
    return std::nullopt;
  }

  return z;
}

glm::u8vec3 AdaptiveRaycaster::compute_color(const glm::mat4& matrix,
                                             const RaycastingTarget& target,
                                             const Camera& camera,
                                             const glm::vec2& pos) {
  std::optional<float> z = compute_intersection(matrix, pos);
  if (!z.has_value()) {
    return m_background_color;
  }

  return compute_phong(
      target.get_material(),
      target.get_normal_vector(glm::vec3(camera.get_projection_view_matrix() *
                                         glm::vec4(pos, *z, 1))),
      glm::normalize(camera.get_position()));
}

glm::u8vec3 AdaptiveRaycaster::compute_phong(const Material& material,
                                             const glm::vec3& normal_vec,
                                             const glm::vec3& view_vec) {
  glm::vec3 light_vec = view_vec;

  float ambient = material.ambient;

  float light_cos = glm::dot(light_vec, normal_vec);
  float diffuse = light_cos > 0 ? material.diffuse * light_cos : 0;

  glm::vec3 reflection_vec =
      2 * glm::dot(light_vec, normal_vec) * normal_vec - light_vec;
  float reflection_view_cos = glm::dot(reflection_vec, view_vec);
  float specular = reflection_view_cos > 0
                       ? material.specular *
                             std::pow(reflection_view_cos, material.shininess)
                       : 0;

  glm::ivec3 color = (ambient + diffuse + specular) * glm::vec3{material.color};
  color.r = std::clamp(color.r, 0, 255);
  color.g = std::clamp(color.g, 0, 255);
  color.b = std::clamp(color.b, 0, 255);
  return color;
}
}  // namespace mge
