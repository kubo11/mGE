#include "render_api/shader_program.hh"

#include "render_context.hh"

namespace {
bool check_compile_erros(unsigned int program_id) {
  int success;
  char log_buffer[1024];
  glGetProgramiv(program_id, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(program_id, sizeof(log_buffer), NULL, log_buffer);
    MGE_ERROR("Shader program linker error \n{}", log_buffer);
  }
  return success;
}

std::vector<std::string> parse_uniform_names(const char* buffer, GLsizei length, GLint size) {
  MGE_ASSERT(size > 0, "");

  if (size == 1) {
    return {buffer};
  }

  MGE_ASSERT(length > 3 && buffer[length - 3] == '[' && buffer[length - 2] == '0' && buffer[length - 1] == ']');
  std::string base_name(buffer, length - 3);

  std::vector<std::string> names(size);
  for (GLint i = 0; i < size; ++i) {
    std::stringstream ss;
    ss << base_name << "[" << i << "]";
    names[i] = ss.str();
  }

  return names;
}

}  // namespace

namespace mge {
ShaderProgram::ShaderProgram() : m_id(glCreateProgram()) {}

ShaderProgram::~ShaderProgram() { release(); }

void ShaderProgram::release() {
  glDeleteProgram(m_id);
  m_id = 0;
}

void ShaderProgram::attach(std::unique_ptr<Shader> shader) {
  MGE_ASSERT(shader != nullptr, "Trying to attach null shader");
  glAttachShader(m_id, shader->get_id());
  m_shaders.insert({shader->get_type(), std::move(shader)});
}

bool ShaderProgram::link() {
  MGE_ASSERT(m_shaders.size() >= 2, "Need at least two shaders to link: {}", m_shaders.size());

  m_uniforms.clear();

  glLinkProgram(m_id);

  if (!check_compile_erros(m_id)) return false;

  load_uniforms();

  return true;
}

void ShaderProgram::bind() {
  RenderContext::get_instance().bind_shader_program(m_id);

  for (const auto& [_, uniform] : m_uniforms) {
    uniform->commit();
  }
}

void ShaderProgram::load_uniforms() {
  GLint size = 0;
  glGetProgramiv(m_id, GL_ACTIVE_UNIFORMS, &size);

  for (GLint i = 0; i < size; ++i) {
    load_uniforms_at_index(i);
  }
}

void ShaderProgram::load_uniforms_at_index(GLuint index) {
  std::array<GLchar, 256> buffer;
  GLsizei length = 0;
  GLint size = 0;
  GLenum type = GL_FLOAT;
  glGetActiveUniform(m_id, index, static_cast<GLsizei>(buffer.size()), &length, &size, &type, buffer.data());

  if (length < 1) {
    MGE_WARN("Unable to get active uniform {} for program {}", index, m_id);
    return;
  }

  std::vector<std::string> names = parse_uniform_names(buffer.data(), length, size);
  for (const std::string& name : names) {
    GLint location = glGetUniformLocation(m_id, name.c_str());
    MGE_ASSERT(location >= 0);
    m_uniforms.emplace(name, create_uniform(name, location, type, m_id));
  }
}

}  // namespace mge
