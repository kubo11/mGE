#include "triangle_layer.hh"

#include <filesystem>
namespace fs = std::filesystem;

void TriangleLayer::configure() {
  auto vertices = std::vector<TriangleVertex>{{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
                                              {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
                                              {{0.0f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}}};
  auto vertex_buffer = std::make_unique<mge::Buffer<TriangleVertex>>();
  vertex_buffer->bind();
  vertex_buffer->copy(vertices);
  vertex_buffer->unbind();
  auto vertex_array =
      std::make_unique<mge::VertexArray<TriangleVertex>>(std::move(vertex_buffer), TriangleVertex::get_attributes());
  mge::RenderPipelineBuilder pipeline_builder;
  m_render_pipeline = std::move(pipeline_builder
                                    .add_shader_program(mge::ShaderSystem::acquire(fs::current_path() / "examples" /
                                                                                   "triangle" / "shaders" / "triangle"))
                                    .build<TriangleVertex>(mge::DrawPrimitiveType::TRIANGLE));
  m_triangle = std::move(std::make_unique<mge::RenderableComponent<TriangleVertex>>(
      std::move(mge::RenderPipelineMap<TriangleVertex>{{mge::RenderMode::SOLID, *m_render_pipeline}}),
      mge::RenderMode::SOLID, std::move(vertex_array)));
}

void TriangleLayer::update() { m_render_pipeline->run(); }