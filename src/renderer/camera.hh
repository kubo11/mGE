#ifndef MGE_RENDERER_CAMERA_HH
#define MGE_RENDERER_CAMERA_HH

#include "../mgepch.hh"

namespace mge {
class Camera {
 public:
  Camera(float view_width, float aspect_ratio, float near_plane,
         float far_plane);

  inline glm::mat4 get_projection_view_matrix() const {
    return m_projection_view_matrix;
  }
  inline glm::vec3 get_position() const {
    return glm::vec3(m_inverse_view_matrix[3][0], m_inverse_view_matrix[3][1],
                     m_inverse_view_matrix[3][2]);
  }
  void add_elevation(float elevation);
  void add_azimuth(float azimuth);
  void add_radius(float radius);
  void move(glm::vec2 pos);
  void zoom(float zoom);
  void set_aspect_ratio(float aspect_ratio);
  inline float get_view_width() const { return m_view_width; }
  void set_view_width(float view_width);

 private:
  float m_view_width;
  float m_aspect_ratio;
  float m_near_plane;
  float m_far_plane;
  glm::vec3 m_target_position = {0.0f, 0.0f, 0.0f};
  float m_radius = 500.0f;
  float m_elevation = 0.0f;
  float m_azimuth = 0.0f;
  glm::mat4 m_inverse_view_matrix{1.0f};
  glm::mat4 m_projection_matrix{1.0f};
  glm::mat4 m_projection_view_matrix{1.0f};

  void update_inverse_view_matrix();
  void update_projection_matrix();
  void update_projection_view_matrix();
};
}  // namespace mge

#endif  // MGE_RENDERER_CAMERA_HH