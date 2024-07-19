#include "render_pipeline_builder.hh"

#include "../utils.hh"

namespace mge {
RenderPipelineBuilder& RenderPipelineBuilder::add_shader_program(std::shared_ptr<ShaderProgram> shader_program) {
  MGE_ASSERT(m_shader_program == nullptr, "Cannot add more than one shader program to render pipeline");
  m_shader_program = shader_program;
  return *this;
}

RenderPipelineBuilder& RenderPipelineBuilder::set_patch_count(unsigned int patch_count) {
  MGE_ASSERT(m_shader_program != nullptr, "Cannot set patch count of render pipeline without a shader program");
  MGE_ASSERT(m_shader_program->has_shader(Shader::Type::TESSELLATION_CONTROL),
             "Cannot set patch count of render pipeline without tessellation control shader");
  MGE_ASSERT(m_shader_program->has_shader(Shader::Type::TESSELLATION_EVALUATION),
             "Cannot set patch count of render pipeline without tessellation evaluation shader");
  m_patch_count = patch_count;
  return *this;
}

void RenderPipelineBuilder::clear() {
  m_shader_program = nullptr;
  m_patch_count = 0;
  auto empty_actions = std::vector<std::unique_ptr<RenderPipelineAction>>{};
  m_buffer_actions.swap(empty_actions);
  empty_actions = std::vector<std::unique_ptr<RenderPipelineAction>>{};
  m_uniform_actions.swap(empty_actions);
}

}  // namespace mge
