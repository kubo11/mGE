#include "instancing_layer.hh"

#include <filesystem>
namespace fs = std::filesystem;

void InstancingLayer::configure() {
  auto vertices = std::vector<QuadVertex>{{{-0.05f, 0.05f}, {1.0f, 0.0f, 0.0f}}, {{-0.05f, -0.05f}, {0.0f, 0.0f, 1.0f}},
                                          {{0.05f, -0.05f}, {0.0f, 1.0f, 0.0f}}, {{-0.05f, 0.05f}, {1.0f, 0.0f, 0.0f}},
                                          {{0.05f, -0.05f}, {0.0f, 1.0f, 0.0f}}, {{0.05f, 0.05f}, {0.0f, 1.0f, 1.0f}}};
  auto vertex_buffer = std::make_unique<mge::Buffer<QuadVertex>>();
  vertex_buffer->bind();
  vertex_buffer->copy(vertices);
  vertex_buffer->unbind();
  auto instance_buffer = std::make_unique<mge::Buffer<QuadInstanceVertex>>();
  auto vertex_array = std::make_unique<mge::InstancedVertexArray<QuadVertex, QuadInstanceVertex>>(
      std::move(vertex_buffer), QuadVertex::get_attributes(), std::move(instance_buffer),
      QuadInstanceVertex::get_attributes());
  mge::RenderPipelineBuilder pipeline_builder;
  m_render_pipeline = std::move(
      pipeline_builder
          .add_shader_program(
              mge::ShaderSystem::acquire(fs::current_path() / "examples" / "instancing" / "shaders" / "instancing"))
          .build_instanced<QuadVertex, QuadInstanceVertex>(mge::DrawPrimitiveType::TRIANGLE, std::move(vertex_array)));
  auto pipelines =
      mge::InstancedRenderPipelineMap<QuadVertex, QuadInstanceVertex>{{mge::RenderMode::SOLID, *m_render_pipeline}};

  float offset = 0.1f;
  for (int y = -10; y < 10; y += 2) {
    for (int x = -10; x < 10; x += 2) {
      QuadInstanceVertex instance_data;

      instance_data.offset.x = (float)x / 10.0f + offset;
      instance_data.offset.y = (float)y / 10.0f + offset;
      m_quads.push_back(std::move(std::unique_ptr<mge::InstancedRenderableComponent<QuadVertex, QuadInstanceVertex>>(
          new mge::InstancedRenderableComponent<QuadVertex, QuadInstanceVertex>(pipelines, mge::RenderMode::SOLID,
                                                                                instance_data))));
    }
  }
}

void InstancingLayer::update() { m_render_pipeline->run(); }