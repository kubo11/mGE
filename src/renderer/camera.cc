#include "camera.hh"

namespace mge {
Camera::Camera(float view_width, float aspect_ratio, float near_plane,
               float far_plane)
    : m_view_width(view_width),
      m_aspect_ratio(aspect_ratio),
      m_near_plane(near_plane),
      m_far_plane(far_plane) {
  update_inverse_view_matrix();
  set_aspect_ratio(aspect_ratio);
}

void Camera::add_elevation(float elevation) {
  m_elevation += elevation;

  float bound = glm::radians(89.0f);
  m_elevation = std::clamp(m_elevation, -bound, bound);

  update_inverse_view_matrix();
  update_projection_view_matrix();
}

void Camera::add_azimuth(float azimuth) {
  m_azimuth += azimuth;

  float pi = glm::pi<float>();
  while (m_azimuth < -pi) {
    m_azimuth += 2 * pi;
  }
  while (m_azimuth >= pi) {
    m_azimuth -= 2 * pi;
  }

  update_inverse_view_matrix();
  update_projection_view_matrix();
}

void Camera::add_radius(float radius) {
  m_radius += radius;
  m_radius = std::max(m_radius, 0.1f);

  update_inverse_view_matrix();
  update_projection_view_matrix();
}

void Camera::move(glm::vec2 pos) {
  m_target_position +=
      m_view_width * glm::mat3{m_inverse_view_matrix} * glm::vec3(pos, 0.0f);

  update_inverse_view_matrix();
  update_projection_view_matrix();
}

void Camera::zoom(float zoom) {
  m_view_width *= zoom;

  update_projection_matrix();
  update_projection_view_matrix();
}

void Camera::set_aspect_ratio(float aspect_ratio) {
  m_aspect_ratio = aspect_ratio;

  update_projection_matrix();
  update_projection_view_matrix();
}

void Camera::set_view_width(float view_width) {
  m_view_width = view_width;

  update_projection_matrix();
  update_projection_view_matrix();
}

void Camera::update_inverse_view_matrix() {
  glm::vec3 position =
      m_target_position +
      m_radius * glm::vec3(std::cos(m_elevation) * std::sin(m_azimuth),
                           std::sin(m_elevation),
                           std::cos(m_elevation) * std::cos(m_azimuth));

  glm::vec3 direction = normalize(position - m_target_position);
  glm::vec3 right =
      glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), direction));
  glm::vec3 up = cross(direction, right);

  m_inverse_view_matrix = {right.x,     right.y,     right.z,     0,
                           up.x,        up.y,        up.z,        0,
                           direction.x, direction.y, direction.z, 0,
                           position.x,  position.y,  position.z,  1};
}

void Camera::update_projection_matrix() {
  float view_height = m_view_width / m_aspect_ratio;
  m_projection_matrix = glm::mat4(
      2 / m_view_width, 0, 0, 0, 0, 2 / view_height, 0, 0, 0, 0,
      -2 / (m_far_plane - m_near_plane), 0, 0, 0,
      -(m_far_plane + m_near_plane) / (m_far_plane - m_near_plane), 1);
}

void Camera::update_projection_view_matrix() {
  m_projection_view_matrix =
      m_inverse_view_matrix * inverse(m_projection_matrix);
}
}  // namespace mge
