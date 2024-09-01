#ifndef MGE_RENDERER_CAMERA
#define MGE_RENDERER_CAMERA

#include "../mgepch.hh"

#include "base_camera.hh"

namespace mge {
class Camera : public BaseCamera {
 public:
  Camera(glm::vec3 position, float yaw, float pitch, float fov, float aspect_ratio, float near_plane, float far_plane);
  ~Camera() = default;

  PREVENT_COPY(Camera);

  virtual glm::mat4 get_view_matrix() override;
  virtual glm::mat4 get_projection_matrix() override;
};
}  // namespace mge

#endif  // MGE_RENDERER_CAMERA