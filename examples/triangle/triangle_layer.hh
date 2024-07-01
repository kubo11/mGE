#ifndef MGE_TRIANGLE_LAYER
#define MGE_TRIANGLE_LAYER

#include "mge.hh"

class TriangleLayer : public mge::Layer {
 public:
  TriangleLayer(mge::Scene& scene);
  ~TriangleLayer() = default;

  virtual void configure() override;
  virtual void update() override;

 private:
  std::unique_ptr<mge::VertexArray> m_vertex_array = nullptr;
  std::shared_ptr<mge::ShaderProgram> m_shader;
  mge::Scene& m_scene;
};

#endif  // MGE_TRIANGLE_LAYER