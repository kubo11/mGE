#include "renderer.hh"

#include "../render_context.hh"

namespace mge {
template <>
void Renderer<RendererType::LINES>::draw(unsigned int vertices_count) {
  glDrawArrays(GL_LINE_STRIP, 0, vertices_count);
  glCheckError();
}

template <>
void Renderer<RendererType::LINES>::draw_indexed(unsigned int indices_count) {
  glDrawElements(GL_LINES, indices_count, GL_UNSIGNED_INT, 0);
  glCheckError();
}

template <>
void Renderer<RendererType::TRIANGLES>::draw(unsigned int vertices_count) {
  glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices_count);
  glCheckError();
}

template <>
void Renderer<RendererType::TRIANGLES>::draw_indexed(
    unsigned int indices_count) {
  glDrawElements(GL_TRIANGLES, indices_count, GL_UNSIGNED_INT, 0);
  glCheckError();
}

template <>
void Renderer<RendererType::PATCHES>::draw(unsigned int vertices_count) {
  glDrawArrays(GL_PATCHES, 0, vertices_count);
  glCheckError();
}

template <>
void Renderer<RendererType::PATCHES>::draw_indexed(unsigned int indices_count) {
  glDrawElements(GL_PATCHES, indices_count, GL_UNSIGNED_INT, 0);
  glCheckError();
}
}  // namespace mge
