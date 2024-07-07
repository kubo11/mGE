#include "render_pipeline_builder.hh"

#include "../utils.hh"

namespace mge {
RenderPipelineBuilder::RenderPipelineBuilder()
    : m_current_pipeline{std::unique_ptr<RenderPipeline>(new RenderPipeline())} {}

RenderPipelineBuilder& RenderPipelineBuilder::add_shader_program(ShaderProgram& shader_program) {
  m_current_pipeline->m_actions.push_back(
      unique_cast<RenderPipelineAction>(std::move(std::make_unique<BindShaderProgramAction>(shader_program))));
  m_has_shader_program = true;
  m_is_patched = shader_program.has_shader(Shader::Type::TESSELLATION_CONTROL) &&
                 shader_program.has_shader(Shader::Type::TESSELLATION_CONTROL);
  return *this;
}

RenderPipelineBuilder& RenderPipelineBuilder::add_vertex_array(VertexArray& vertex_array) {
  m_current_pipeline->m_actions.push_back(
      unique_cast<RenderPipelineAction>(std::move(std::make_unique<BindVertexArrayAction>(vertex_array))));
  m_has_vertex_array = true;
  m_next_draw_call_size = vertex_array.get_count();
  m_instance_count = vertex_array.get_instance_count();
  return *this;
}

RenderPipelineBuilder& RenderPipelineBuilder::add_draw_call(DrawPrimitiveType type, unsigned int size, bool indexed,
                                                            unsigned int instances, unsigned int patches) {
  MGE_ASSERT(m_has_vertex_array, "Cannot add draw call to render pipeline without vertex array");
  MGE_ASSERT(m_has_shader_program, "Cannot add draw call to render pipeline without shader program");
  m_current_pipeline->m_actions.push_back(unique_cast<RenderPipelineAction>(
      std::move(std::make_unique<SendDrawCallAction>(type, size, indexed, instances, patches))));
  m_has_draw_call = true;
  return *this;
}

std::unique_ptr<RenderPipeline> RenderPipelineBuilder::build() {
  if (!m_has_draw_call) {
    generate_draw_call();
  }
  auto pipeline = std::move(m_current_pipeline);
  clear();
  return pipeline;
}

void RenderPipelineBuilder::clear() {
  m_has_vertex_array = false;
  m_has_shader_program = false;
  m_is_patched = false;
  m_is_element_buffer_bound = false;
  m_has_draw_call = false;
  m_next_draw_call_size = 0;
  m_instance_count = 0;
  m_current_pipeline.reset(new RenderPipeline());
}

void RenderPipelineBuilder::generate_draw_call() {
  MGE_ASSERT(!m_is_patched, "Cannot deduce number of patches for render pipeline");
  MGE_WARN("Using TRIANGLE as default draw primitive for render pipeline.");
  add_draw_call(DrawPrimitiveType::TRIANGLE, m_next_draw_call_size, m_is_element_buffer_bound, m_instance_count);
}
}  // namespace mge
