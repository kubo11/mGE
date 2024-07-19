#ifndef MGE_TRIANGLE_LAYER
#define MGE_TRIANGLE_LAYER

#include "mge.hh"

struct TriangleVertex {
  glm::vec3 position;
  glm::vec3 color;

  inline static std::vector<mge::VertexAttribute> get_attributes() {
    return {{.size = 3u, .type = GL_FLOAT}, {.size = 3u, .type = GL_FLOAT}};
  }
};

class TriangleLayer : public mge::Layer {
 public:
  TriangleLayer() = default;
  ~TriangleLayer() = default;

  virtual void configure() override;
  virtual void update() override;

 private:
  std::unique_ptr<mge::RenderPipeline<TriangleVertex>> m_render_pipeline = nullptr;
  std::unique_ptr<mge::RenderableComponent<TriangleVertex>> m_triangle = nullptr;
};

#endif  // MGE_TRIANGLE_LAYER