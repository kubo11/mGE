#ifndef MGE_RENDERER_ADAPTIVE_RAYCASTER_HH
#define MGE_RENDERER_ADAPTIVE_RAYCASTER_HH

#include "../mgepch.hh"

#include "camera.hh"
#include "cpu_bitmap.hh"
#include "gpu_canvas.hh"
#include "material.hh"

namespace mge {
class RaycastingTarget {
 public:
  RaycastingTarget(Material material);
  virtual ~RaycastingTarget() {}

  inline const Material& get_material() const { return m_material; }
  virtual const glm::mat4& get_matrix() const = 0;
  virtual const glm::vec3 get_normal_vector(const glm::vec3& point) const = 0;

 private:
  Material m_material;
};

class AdaptiveRaycaster {
 public:
  AdaptiveRaycaster(uint8_t max_pixel_size_exponent, unsigned int width,
                    unsigned int height, glm::u8vec3 background_color);
  void reset();
  void update(const RaycastingTarget& target, const Camera& camera);
  void draw(const RaycastingTarget& target, const Camera& camera);

  inline void set_max_pixel_size_exponent(uint8_t max_pixel_size_exponent) {
    m_max_pixel_size_exponent = max_pixel_size_exponent;
  }

 private:
  CPUBitmap m_bitmap;
  GPUCanvas m_canvas;
  uint8_t m_max_pixel_size_exponent;
  uint8_t m_pixel_size;
  glm::u8vec3 m_background_color;
  float m_width;
  float m_height;

  uint8_t get_max_pixel_size() const;
  std::optional<float> compute_intersection(const glm::mat4& matrix,
                                            const glm::vec2& pos);
  glm::u8vec3 compute_color(const glm::mat4& matrix,
                            const RaycastingTarget& target,
                            const Camera& camera, const glm::vec2& pos);
  glm::u8vec3 compute_phong(const Material& material,
                            const glm::vec3& normal_vec,
                            const glm::vec3& view_vec);
};
}  // namespace mge

#endif  // MGE_RENDERER_ADAPTIVE_RAYCASTER_HH