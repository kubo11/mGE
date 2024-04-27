#include "shader.hh"

namespace mge {
GLuint Shader::s_current_shader_id = 0;

Shader::Shader(const std::unordered_map<GLenum, fs::path>& shader_paths)
    : m_path(fs::path(shader_paths.begin()->second).replace_extension()) {
  std::vector<unsigned int> ids;
  ids.reserve(shader_paths.size());
  for (auto& [type, path] : shader_paths) {
    auto id = glCreateShader(type);
    std::string code = Shader::read_shader_code(path);
    const char* code_c = code.c_str();
    glShaderSource(id, 1, &code_c, NULL);
    glCompileShader(id);
    Shader::check_shader_compile_erros(id);
    ids.emplace_back(id);
  }

  // shader Program
  m_id = glCreateProgram();
  for (auto& id : ids) {
    glAttachShader(m_id, id);
  }
  glLinkProgram(m_id);
  Shader::check_program_compile_erros(m_id);

  for (auto& id : ids) {
    glDeleteShader(id);
  }

  MGE_INFO("Created shader {}", m_path.string());

  glCheckError();
}

Shader::~Shader() {
  if (m_id) {
    glDeleteProgram(m_id);
    glCheckError();
    if (s_current_shader_id == m_id) {
      s_current_shader_id = 0;
    }
    MGE_INFO("Destroyed shader {}", m_path.string());
  }
}

void Shader::use() const {
  if (s_current_shader_id != m_id) {
    glUseProgram(m_id);
    s_current_shader_id = m_id;
    glCheckError();
  }
}

bool Shader::is_used() const { return s_current_shader_id == m_id; }

void Shader::unuse() const {
  if (s_current_shader_id != 0) {
    glUseProgram(0);
    s_current_shader_id = 0;
    glCheckError();
  }
}

GLint Shader::get_uniform_id(const std::string& name) {
  auto id = glGetUniformLocation(m_id, name.c_str());
  glCheckError();
  if (id == -1) {
    MGE_ERROR("No uniform named {} found in {} shader", name, m_path.string());
  }
  return id;
}

void Shader::set_uniform(const std::string& name, bool value) {
  glUniform1i(get_uniform_id(name), static_cast<int>(value));
  glCheckError();
}

void Shader::set_uniform(const std::string& name, int value) {
  glUniform1i(get_uniform_id(name), value);
  glCheckError();
}

void Shader::set_uniform(const std::string& name, float value) {
  glUniform1f(get_uniform_id(name), value);
  glCheckError();
}

void Shader::set_uniform(const std::string& name, const glm::vec2& value) {
  glUniform2fv(get_uniform_id(name), 1, &value[0]);
  glCheckError();
}

void Shader::set_uniform(const std::string& name, const glm::vec3& value) {
  glUniform3fv(get_uniform_id(name), 1, &value[0]);
  glCheckError();
}

void Shader::set_uniform(const std::string& name, const glm::mat4& value) {
  glUniformMatrix4fv(get_uniform_id(name), 1, GL_FALSE, &value[0][0]);
  glCheckError();
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
