#ifndef MGE_RENDERER_GPU_CANVAS_HH
#define MGE_RENDERER_GPU_CANVAS_HH

#include "../mgepch.hh"
#include "../render_api/shader.hh"
#include "cpu_bitmap.hh"

namespace mge {
class GPUCanvas {
 public:
  GPUCanvas();
  void init(const CPUBitmap& bitmap);
  void update(const CPUBitmap& bitmap);
  void draw();

 private:
  std::unique_ptr<mge::Shader> m_texture_shader;
  unsigned int m_texture_id;
  unsigned int m_vao_id;
};
}  // namespace mge

#endif  // MGE_RENDERER_GPU_CANVAS_HH