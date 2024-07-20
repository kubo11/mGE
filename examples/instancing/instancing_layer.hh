#ifndef MGE_INSTANCING_LAYER
#define MGE_INSTANCING_LAYER

#include "mge.hh"

struct QuadVertex {
  glm::vec2 position;
  glm::vec3 color;

  inline static std::vector<mge::VertexAttribute> get_attributes() {
    return {{.size = 2u, .type = GL_FLOAT}, {.size = 3u, .type = GL_FLOAT}};
  }
};

struct QuadInstanceVertex {
  glm::vec2 offset;

  inline static std::vector<mge::VertexInstanceAttribute> get_attributes() {
    return {{.size = 2u, .type = GL_FLOAT, .divisor = 1}};
  }
};

class InstancingLayer : public mge::Layer {
 public:
  InstancingLayer() = default;
  ~InstancingLayer() = default;

  virtual void configure() override;
  virtual void update() override;

 private:
  std::unique_ptr<mge::InstancedRenderPipeline<QuadVertex, QuadInstanceVertex>> m_render_pipeline = nullptr;
  std::vector<std::unique_ptr<mge::InstancedRenderableComponent<QuadVertex, QuadInstanceVertex>>> m_quads;
};

#endif  // MGE_INSTANCING_LAYER