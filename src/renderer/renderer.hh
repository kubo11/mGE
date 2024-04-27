#ifndef MGE_RENDERER_RENDERER_HH
#define MGE_RENDERER_RENDERER_HH

#include "../mgepch.hh"

namespace mge {

enum class RendererType { LINES, TRIANGLES, PATCHES };

template <RendererType type>
class Renderer {
 public:
  static void draw(unsigned int vertices_count);
  static void draw_indexed(unsigned int indices_count);
};

}  // namespace mge

#endif  // MGE_RENDERER_RENDERER_HH