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
  TriangleLayer();
  ~TriangleLayer() = default;

  virtual void configure() override;
  virtual void update() override;

 private:
  std::unique_ptr<mge::VertexArray> m_vertex_array = nullptr;
  std::unique_ptr<mge::Buffer<TriangleVertex>> m_vertex_buffer = nullptr;
  std::shared_ptr<mge::ShaderProgram> m_shader = nullptr;
};

#endif  // MGE_TRIANGLE_LAYER