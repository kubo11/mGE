#include "camera.hh"

namespace mge {
Camera::Camera(glm::vec3 position, float yaw, float pitch, float fov, float aspect_ratio, float near_plane,
               float far_plane)
    : BaseCamera(position, yaw, pitch, fov, aspect_ratio, near_plane, far_plane) {
  m_view = glm::lookAt(m_pos, m_pos + m_front, m_up);
  m_projection = glm::perspective(glm::radians(m_fov), m_aspect_ratio, m_near_plane, m_far_plane);
}

glm::mat4 Camera::get_view_matrix() {
  if (m_update_camera_vectors) {
    m_update_camera_vectors = false;
    update_camera_vectors();
  }
  if (m_update_view) {
    m_update_view = false;
    m_view = glm::lookAt(m_pos, m_pos + m_front, m_up);
  }

  return m_view;
}

glm::mat4 Camera::get_projection_matrix() {
  if (m_update_projection) {
    m_update_projection = false;
    m_projection = glm::perspective(glm::radians(m_fov), m_aspect_ratio, m_near_plane, m_far_plane);
  }

  return m_projection;
}
}  // namespace mge
