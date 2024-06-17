#include "shader.hh"

namespace {
bool check_compile_erros(unsigned int shader_id) {
  int success;
  char log_buffer[1024];
  glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader_id, sizeof(log_buffer), NULL, log_buffer);
    MGE_ERROR("Shader compilation error \n{}", log_buffer);
  }
  return success;
}
}  // namespace

namespace mge {
Shader::Shader(Shader::Type type) : m_id(glCreateShader(to_gl_shader_type(type))), m_type(type) {}

Shader::~Shader() { release(); }

void Shader::release() { glDeleteShader(m_id); }

bool Shader::compile(const char *source) {
  glShaderSource(m_id, 1, &source, nullptr);
  glCompileShader(m_id);

  return check_compile_erros(m_id);
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
