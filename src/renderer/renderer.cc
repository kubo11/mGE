#include "renderer.hh"

#include "../render_context.hh"

namespace mge {
template <>
void Renderer<RendererType::LINES>::draw(unsigned int vertices_count) {
  glDrawArrays(GL_LINES, 0, vertices_count);
  glCheckError();
}

template <>
void Renderer<RendererType::LINES>::draw_indexed(unsigned int indices_count) {
  glDrawElements(GL_LINES, indices_count, GL_UNSIGNED_INT, 0);
  glCheckError();
}

template <>
void Renderer<RendererType::TRIANGLES>::draw(unsigned int vertices_count) {
  glDrawArrays(GL_LINES, 0, vertices_count);
  glCheckError();
}

template <>
void Renderer<RendererType::TRIANGLES>::draw_indexed(
    unsigned int indices_count) {
  glDrawElements(GL_TRIANGLES, indices_count, GL_UNSIGNED_INT, 0);
  glCheckError();
}
}  // namespace mge
