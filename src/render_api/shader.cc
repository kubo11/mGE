#include "shader.hh"

namespace mge {
Shader::Shader(const fs::path& vertex_path, const fs::path& fragment_path)
    : m_path(fs::path(vertex_path).replace_extension()) {
  // vertex shader
  unsigned int vertex_id = glCreateShader(GL_VERTEX_SHADER);
  std::string vertex_code = Shader::read_shader_code(vertex_path);
  const char* vertex_code_c = vertex_code.c_str();
  glShaderSource(vertex_id, 1, &vertex_code_c, NULL);
  glCompileShader(vertex_id);
  Shader::check_shader_compile_erros(vertex_id);

  // fragment Shader
  unsigned int fragment_id = glCreateShader(GL_FRAGMENT_SHADER);
  std::string fragment_code = Shader::read_shader_code(fragment_path);
  const char* fragment_code_c = fragment_code.c_str();
  glShaderSource(fragment_id, 1, &fragment_code_c, NULL);
  glCompileShader(fragment_id);
  Shader::check_shader_compile_erros(fragment_id);

  // shader Program
  m_id = glCreateProgram();
  glAttachShader(m_id, vertex_id);
  glAttachShader(m_id, fragment_id);
  glLinkProgram(m_id);
  Shader::check_program_compile_erros(m_id);

  glDeleteShader(vertex_id);
  glDeleteShader(fragment_id);
}

void Shader::use() { glUseProgram(m_id); }

void Shader::set_uniform(const std::string& name, bool value) {
  glUniform1i(glGetUniformLocation(m_id, name.c_str()),
              static_cast<int>(value));
}

void Shader::set_uniform(const std::string& name, int value) {
  glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
}

void Shader::set_uniform(const std::string& name, float value) {
  glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
}

void Shader::set_uniform(const std::string& name, const glm::vec3& value) {
  glUniform3fv(glGetUniformLocation(m_id, name.c_str()), 1, &value[0]);
}

void Shader::set_uniform(const std::string& name, const glm::mat4& value) {
  glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE,
                     &value[0][0]);
}

void Shader::check_shader_compile_erros(unsigned int shader_id) {
  int success;
  char log_buffer[1024];
  glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader_id, sizeof(log_buffer), NULL, log_buffer);
    MGE_ERROR("Shader compilation error \n{}", log_buffer);
  }
}

void Shader::check_program_compile_erros(unsigned int program_id) {
  int success;
  char log_buffer[1024];
  glGetProgramiv(program_id, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(program_id, sizeof(log_buffer), NULL, log_buffer);
    MGE_ERROR("Shader program linker error \n{}", log_buffer);
  }
}

std::string Shader::read_shader_code(const fs::path& path) {
  std::string code;
  std::ifstream file;

  file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    file.open(path);
    std::stringstream stream;
    stream << file.rdbuf();
    file.close();
    code = stream.str();
  } catch (std::ifstream::failure& e) {
    MGE_ERROR("Could not read shader file: {}", e.what());
  }

  return code;
}

}  // namespace mge
