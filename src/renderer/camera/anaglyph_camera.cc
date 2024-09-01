#include "anaglyph_camera.hh"

namespace mge {
AnaglyphCamera::AnaglyphCamera(glm::vec3 position, float yaw, float pitch, float fov, float aspect_ratio,
                               float near_plane, float far_plane, float eye_distance, float projective_plane_distance,
                               float screen_distance)
    : BaseCamera(position, yaw, pitch, fov, aspect_ratio, near_plane, far_plane),
      m_eye(Eye::Left),
      m_eye_distance(eye_distance),
      m_projective_plane_distance(projective_plane_distance),
      m_screen_distance(screen_distance) {
  m_view = create_view_matrix();
  m_projection = create_projection_matrix();
}

glm::mat4 AnaglyphCamera::get_view_matrix() {
  if (m_update_camera_vectors) {
    m_update_camera_vectors = false;
    update_camera_vectors();
  }
  if (m_update_view) {
    m_update_view = false;
    m_view = create_view_matrix();
  }

  return m_view;
}

glm::mat4 AnaglyphCamera::get_projection_matrix() {
  if (m_update_projection) {
    m_update_projection = false;
    m_projection = create_projection_matrix();
  }

  return m_projection;
}

AnaglyphCamera::Eye AnaglyphCamera::get_eye() const { return m_eye; }

void AnaglyphCamera::set_eye(Eye eye) {
  if (m_eye == eye) return;
  m_eye = eye;
  m_update_view = true;
}

float AnaglyphCamera::get_eye_distance() const { return m_eye_distance; }

void AnaglyphCamera::set_eye_distance(float distance) {
  m_eye_distance = distance;
  m_update_view = true;
  m_update_projection = true;
}

float AnaglyphCamera::get_projective_plane_distance() const { return m_projective_plane_distance; }

void AnaglyphCamera::set_projective_plane_distance(float distance) {
  m_projective_plane_distance = distance;
  m_update_view = true;
  m_update_projection = true;
}

float AnaglyphCamera::get_screen_distance() const { return m_screen_distance; }

void AnaglyphCamera::set_screen_distance(float distance) {
  m_screen_distance = distance;
  m_update_view = true;
  m_update_projection = true;
}

glm::mat4 AnaglyphCamera::create_view_matrix() {
  float virtual_eye_distance = m_projective_plane_distance / m_screen_distance * m_eye_distance;
  glm::vec3 eye_pos;
  if (m_eye == Eye::Left) {
    eye_pos = m_pos - m_right * virtual_eye_distance / 2.0f;

  } else {
    eye_pos = m_pos + m_right * virtual_eye_distance / 2.0f;
  }
  return glm::inverse(glm::mat4{m_right.x, m_right.y, m_right.z, 0.0f, m_up.x, m_up.y, m_up.z, 0.0f, -m_front.x,
                                -m_front.y, -m_front.z, 0, eye_pos.x, eye_pos.y, eye_pos.z, 1.0f});
}

glm::mat4 AnaglyphCamera::create_projection_matrix() {
  float fov_rad = glm::radians(m_fov);
  float cot = std::cos(fov_rad / 2.0f) / std::sin(fov_rad / 2.0f);
  float eye_offset = m_eye_distance * cot / (2.0f * m_aspect_ratio * m_screen_distance);

  if (m_eye == Eye::Left) {
    return {cot / m_aspect_ratio,
            0.0f,
            0.0f,
            0.0f,
            0.0f,
            cot,
            0.0f,
            0.0f,
            eye_offset,
            0.0f,
            -(m_far_plane + m_near_plane) / (m_far_plane - m_near_plane),
            -1.0f,
            0.0f,
            0.0f,
            -2.0f * m_far_plane * m_near_plane / (m_far_plane - m_near_plane),
            0.0f};
  } else {
    return {cot / m_aspect_ratio,
            0.0f,
            0.0f,
            0.0f,
            0.0f,
            cot,
            0.0f,
            0.0f,
            -eye_offset,
            0.0f,
            -(m_far_plane + m_near_plane) / (m_far_plane - m_near_plane),
            -1.0f,
            0.0f,
            0.0f,
            -2.0f * m_far_plane * m_near_plane / (m_far_plane - m_near_plane),
            0.0f};
  }
}

}  // namespace mge
