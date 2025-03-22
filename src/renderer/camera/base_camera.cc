#include "base_camera.hh"

namespace mge {
BaseCamera::BaseCamera(glm::vec3 position, float yaw, float pitch, float fov, float aspect_ratio, float near_plane,
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
      m_update_camera_vectors(false),
      m_rotation_sensitivity(100000.0f),
      m_zoom_sensitivity(400.0f),
      m_velocity(20.0f) {
  update_camera_vectors();
}

void BaseCamera::move(BaseCamera::MoveDirection dir, float dt) {
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

void BaseCamera::move(glm::vec3 offset, float dt) {
  if (offset.x > 0) {
    move(MoveDirection::RIGHT, dt);
  } else if (offset.x < 0) {
    move(MoveDirection::LEFT, dt);
  }

  if (offset.y > 0) {
    move(MoveDirection::UP, dt);
  } else if (offset.y < 0) {
    move(MoveDirection::DOWN, dt);
  }

  if (offset.z > 0) {
    move(MoveDirection::FRONT, dt);
  } else if (offset.z < 0) {
    move(MoveDirection::BACK, dt);
  }
}

void BaseCamera::rotate(float yaw, float pitch, float dt) {
  m_yaw += yaw * m_rotation_sensitivity * dt;
  while (m_yaw > 360.0f) m_yaw -= 360.0f;
  while (m_yaw < 0.0f) m_yaw += 360.0f;
  m_pitch = std::clamp(m_pitch + pitch * m_rotation_sensitivity * dt, -89.0f, 89.0f);
  m_update_view = true;
  m_update_camera_vectors = true;
}

void BaseCamera::zoom(float zoom_amount, float dt) {
  if (zoom_amount < 1.0f) {
    m_pos += m_front * m_zoom_sensitivity * dt;
  } else {
    m_pos -= m_front * m_zoom_sensitivity * dt;
  }
  m_update_view = true;
}

void BaseCamera::update_camera_vectors() {
  glm::vec3 front;
  front.x = glm::cos(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));
  front.y = glm::sin(glm::radians(m_pitch));
  front.z = glm::sin(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));
  m_front = glm::normalize(front);
  m_right = glm::normalize(glm::cross(m_front, m_world_up));
  m_up = glm::normalize(glm::cross(m_right, m_front));
}

void BaseCamera::copy_camera_data(BaseCamera& other) {
  m_velocity = other.m_velocity;
  m_rotation_sensitivity = other.m_rotation_sensitivity;
  m_zoom_sensitivity = other.m_zoom_sensitivity;
  m_aspect_ratio = other.m_aspect_ratio;
  m_near_plane = other.m_near_plane;
  m_far_plane = other.m_far_plane;
  m_pos = other.m_pos;
  m_front = other.m_front;
  m_up = other.m_up;
  m_right = other.m_right;
  m_world_up = other.m_world_up;
  m_yaw = other.m_yaw;
  m_pitch = other.m_pitch;
  m_fov = other.m_fov;
  m_update_view = true;
  m_update_projection = true;
  m_update_camera_vectors = true;
}
}  // namespace mge
