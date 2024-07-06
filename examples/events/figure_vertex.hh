#ifndef MGE_FIGURE_VERTEX
#define MGE_FIGURE_VERTEX

#include "mge.hh"

struct FigureVertex {
  glm::vec3 position;

  inline static std::vector<mge::VertexAttribute> get_attributes() { return {{.size = 3u, .type = GL_FLOAT}}; }
};

#endif  // MGE_FIGURE_VERTEX