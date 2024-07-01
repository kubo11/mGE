#include "render_api/shader_program.hh"

#include "render_context.hh"

namespace mge {
ShaderProgram::ShaderProgram() : m_id(RenderContext::get_instance().create_shader_program()) {}

ShaderProgram::~ShaderProgram() { release(); }

void ShaderProgram::release() {
  if (m_id) {
    RenderContext::get_instance().destroy_shader_program(m_id);
    m_id = 0;
  }
}

void ShaderProgram::attach(std::unique_ptr<Shader> shader) {
  MGE_ASSERT(shader != nullptr, "Trying to attach null shader");
  RenderContext::get_instance().attach_shader(m_id, shader->get_id());
  m_shaders.insert({shader->get_type(), std::move(shader)});
}

void ShaderProgram::link() {
  MGE_ASSERT(m_shaders.size() >= 2, "Need at least two shaders to link: {}", m_shaders.size());

  m_uniforms.clear();

  RenderContext::get_instance().link_shader_program(m_id);

  load_uniforms();
}

void ShaderProgram::bind() {
  RenderContext::get_instance().bind_shader_program(m_id);

  for (const auto& [_, uniform] : m_uniforms) {
    uniform->commit();
  }
}

void ShaderProgram::unbind() {
  RenderContext::get_instance().unbind_shader_program(m_id);
}

void ShaderProgram::load_uniforms() {
  auto uniforms = RenderContext::get_instance().get_uniforms(m_id);
  for (const auto& [name, type] : uniforms) {
    auto location = RenderContext::get_instance().get_uniform_location(m_id, name);
    m_uniforms.emplace(name, create_uniform(name, location, type, m_id));
  }
}
}  // namespace mge
