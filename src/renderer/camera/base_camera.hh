#ifndef MGE_RENDERER_BASE_CAMERA
#define MGE_RENDERER_BASE_CAMERA

#include "../mgepch.hh"

namespace mge {
class BaseCamera {
 public:
  enum class MoveDirection { FRONT, BACK, RIGHT, LEFT, UP, DOWN };
  BaseCamera(glm::vec3 position, float yaw, float pitch, float fov, float aspect_ratio, float near_plane,
             float far_plane);
  ~BaseCamera() = default;

  PREVENT_COPY(BaseCamera);

  virtual glm::mat4 get_view_matrix() = 0;
  virtual glm::mat4 get_projection_matrix() = 0;

  void move(MoveDirection dir, float dt);
  void move(glm::vec3 offset, float dt);
  void rotate(float yaw, float pitch, float dt);
  void zoom(float zoom_amount, float dt);

  void copy_camera_data(BaseCamera& other);

  const float get_velocity() const { return m_velocity; }
  const float get_rotation_sensitivity() const { return m_rotation_sensitivity; }
  const float get_zoom_sensitivity() const { return m_zoom_sensitivity; }
  const float get_aspect_ratio() const { return m_aspect_ratio; }
  const float get_near_plane() const { return m_near_plane; }
  const float get_far_plane() const { return m_far_plane; }
  const glm::vec3 get_position() const { return m_pos; }
  const glm::vec3 get_front() const { return m_front; }
  const glm::vec3 get_up() const { return m_up; }
  const glm::vec3 get_right() const { return m_right; }
  const glm::vec3 get_world_up() const { return m_world_up; }
  const float get_yaw() const { return m_yaw; }
  const float get_pitch() const { return m_pitch; }
  const float get_fov() const { return m_fov; }

  void set_velocity(float velocity) { m_velocity = velocity; }
  void set_rotation_sensitivity(float rotation_sensitivity) { m_rotation_sensitivity = rotation_sensitivity; }
  void set_zoom_sensitivity(float zoom_sensitivity) { m_zoom_sensitivity = zoom_sensitivity; }
  void set_aspect_ratio(float aspect_ratio) { m_aspect_ratio = aspect_ratio; }
  void set_near_plane(float near_plane) { m_near_plane = near_plane; }
  void set_far_plane(float far_plane) { m_far_plane = far_plane; }
  void set_position(const glm::vec3& pos) { m_pos = pos; }
  void set_front(const glm::vec3& front) { m_front = front; }
  void set_up(const glm::vec3& up) { m_up = up; }
  void set_right(const glm::vec3& right) { m_right = right; }
  void set_world_up(const glm::vec3& world_up) { m_world_up = world_up; }
  void set_yaw(float yaw) { m_yaw = yaw; }
  void set_pitch(float pitch) { m_pitch = pitch; }
  void set_fov(float fov) { m_fov = fov; }

 protected:
  float m_velocity;
  float m_rotation_sensitivity;
  float m_zoom_sensitivity;

  float m_aspect_ratio;
  float m_near_plane;
  float m_far_plane;

  glm::vec3 m_pos;
  glm::vec3 m_front;
  glm::vec3 m_up;
  glm::vec3 m_right;
  glm::vec3 m_world_up;

  float m_yaw;
  float m_pitch;
  float m_fov;

  glm::mat4 m_view;
  bool m_update_view;
  glm::mat4 m_projection;
  bool m_update_projection;
  bool m_update_camera_vectors;

  void update_camera_vectors();
};
}  // namespace mge

#endif  // MGE_RENDERER_BASE_CAMERA