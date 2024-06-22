#include "camera.hh"

namespace mge {
Camera::Camera(glm::vec3 position, float yaw, float pitch, float fov, float aspect_ratio, float near_plane,
               float far_plane)
    : m_pos(position),
      m_world_up(glm::vec3(0.0f, 1.0f, 0.0f)),
      m_yaw(yaw),
      m_pitch(pitch),
      m_fov(fov),
      m_aspect_ratio(aspect_ratio),
      m_near_plane(near_plane),
      m_far_plane(far_plane),
      m_update_view(false),
      m_update_projection(false),
      m_update_camera_vectors(false) {
  update_camera_vectors();
}

const glm::mat4& Camera::get_view_matrix() {
  if (m_update_camera_vectors) {
    m_update_camera_vectors = false;
    update_camera_vectors();
  }
  if (m_update_view) {
    m_update_view = false;
    m_view = glm::lookAt(m_pos, m_front, m_up);
  }

  return m_view;
}

const glm::mat4& Camera::get_projection_matrix() {
  if (m_update_projection) {
    m_update_projection = false;
    m_projection = glm::perspective(glm::radians(m_fov), m_aspect_ratio, m_near_plane, m_far_plane);
  }

  return m_projection;
}

void Camera::move(Camera::MoveDirection dir, float dt) {
  switch (dir) {
    case MoveDirection::FRONT:
      m_pos += m_front * m_velocity * dt;
      break;
    case MoveDirection::BACK:
      m_pos -= m_front * m_velocity * dt;
      break;
    case MoveDirection::RIGHT:
      m_pos += m_right * m_velocity * dt;
      break;
    case MoveDirection::LEFT:
      m_pos -= m_right * m_velocity * dt;
      break;
    case MoveDirection::UP:
      m_pos += m_up * m_velocity * dt;
      break;
    case MoveDirection::DOWN:
      m_pos -= m_up * m_velocity * dt;
      break;
  }
  m_update_view = true;
}

void Camera::rotate(float yaw, float pitch) {
  m_yaw = std::clamp(m_yaw + yaw * m_rotation_sensitivity, -89.0f, 89.0f);
  m_pitch = std::clamp(m_pitch + pitch * m_rotation_sensitivity, -89.0f, 89.0f);
  m_update_view = true;
  m_update_camera_vectors = true;
}

void Camera::zoom(float zoom_amount) {
  m_fov = std::clamp(m_fov + zoom_amount, 1.0f, 45.0f);
  m_update_projection = true;
}

void Camera::update_camera_vectors() {
  glm::vec3 front;
  front.x = glm::cos(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));
  front.y = glm::sin(glm::radians(m_pitch));
  front.z = glm::sin(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));
  m_front = glm::normalize(front);
  m_right = glm::normalize(glm::cross(m_front, m_world_up));
  m_up = glm::normalize(glm::cross(m_right, m_front));
}
}  // namespace mge
