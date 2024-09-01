#ifndef MGE_RENDERER_ANAGLYPH_CAMERA
#define MGE_RENDERER_ANAGLYPH_CAMERA

#include "../mgepch.hh"

#include "base_camera.hh"

namespace mge {
class AnaglyphCamera : public BaseCamera {
 public:
  enum class Eye { Left, Right };
  AnaglyphCamera(glm::vec3 position, float yaw, float pitch, float fov, float aspect_ratio, float near_plane,
                 float far_plane, float eye_distance, float projective_plane_distance, float screen_distance);
  ~AnaglyphCamera() = default;

  PREVENT_COPY(AnaglyphCamera);

  virtual glm::mat4 get_view_matrix() override;
  virtual glm::mat4 get_projection_matrix() override;

  Eye get_eye() const;
  void set_eye(Eye eye);
  float get_eye_distance() const;
  void set_eye_distance(float distance);
  float get_projective_plane_distance() const;
  void set_projective_plane_distance(float distance);
  float get_screen_distance() const;
  void set_screen_distance(float distance);

 private:
  glm::mat4 m_view;
  glm::mat4 m_projection;
  Eye m_eye;
  float m_eye_distance;
  float m_projective_plane_distance;
  float m_screen_distance;

  glm::mat4 create_view_matrix();
  glm::mat4 create_projection_matrix();
};
}  // namespace mge

#endif  // MGE_RENDERER_ANAGLYPH_CAMERA