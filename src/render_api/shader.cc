#include "shader.hh"

namespace mge {
Shader::Shader(Shader::Type type)
    : m_id(RenderContext::get_instance().create_shader(to_gl_shader_type(type))), m_type(type) {}

Shader::~Shader() { release(); }

void Shader::release() {
  if (m_id) {
    RenderContext::get_instance().destroy_shader(m_id);
    m_id = 0;
  }
}

void Shader::compile(const char *source) {
  RenderContext::get_instance().set_shader_source(m_id, source);
  RenderContext::get_instance().compile_shader(m_id);
}

GLenum Shader::to_gl_shader_type(Shader::Type type) {
  switch (type) {
    case Shader::Type::VERTEX:
      return GL_VERTEX_SHADER;
    case Shader::Type::TESSELLATION_CONTROL:
      return GL_TESS_CONTROL_SHADER;
    case Shader::Type::TESSELLATION_EVALUATION:
      return GL_TESS_EVALUATION_SHADER;
    case Shader::Type::GEOMETRY:
      return GL_GEOMETRY_SHADER;
    case Shader::Type::FRAGMENT:
      return GL_FRAGMENT_SHADER;
    case Shader::Type::COMPUTE:
      return GL_COMPUTE_SHADER;

    default:
      return GL_NONE;
  }
}

}  // namespace mge
