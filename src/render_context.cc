#include "render_context.hh"

namespace {
bool check_shader_compile_erros(unsigned int shader_id) {
  int success;
  char log_buffer[1024];
  glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader_id, sizeof(log_buffer), NULL, log_buffer);
    MGE_ERROR("Shader compilation error \n{}", log_buffer);
  }
  return success;
}

bool check_shader_program_compile_erros(unsigned int program_id) {
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
};  // namespace

namespace mge {
std::shared_ptr<RenderContext> RenderContext::s_instance = nullptr;

RenderContext::RenderContext() {
  s_instance->init();
  MGE_INFO("Render context created");
}

RenderContext::~RenderContext() {
  if (s_instance) {
    s_instance->terminate();
  }
}

std::shared_ptr<RenderContext> RenderContext::create() {
  MGE_ASSERT(s_instance == nullptr, "Render context already exists!");

  s_instance = std::shared_ptr<RenderContext>(new RenderContext());
  return s_instance;
}

void RenderContext::init() {
  auto res = gladLoadGL(glfwGetProcAddress);
  MGE_ASSERT(res != 0, "Failed to initialize GLAD");

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glEnable(GL_MULTISAMPLE);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  MGE_INFO("Render context initialized");
}

void RenderContext::terminate() {
  s_instance = nullptr;
  MGE_INFO("Render context terminated");
}

size_t RenderContext::glSizeofType(GLenum type) {
  switch (type) {
    case GL_BYTE:
    case GL_UNSIGNED_BYTE:
      return sizeof(GLbyte);
    case GL_SHORT:
    case GL_UNSIGNED_SHORT:
      return sizeof(GLshort);
    case GL_INT:
    case GL_UNSIGNED_INT:
      return sizeof(GLint);
    case GL_FLOAT:
      return sizeof(GLfloat);
    case GL_DOUBLE:
      return sizeof(GLdouble);
    case GL_HALF_FLOAT:
      return sizeof(GLhalf);
  }

  return 0;
}

GLenum RenderContext::glCheckError_(const char* file, int line) {
  GLenum errorCode;
  while ((errorCode = glGetError()) != GL_NO_ERROR) {
    std::string error;
    switch (errorCode) {
      case GL_INVALID_ENUM:
        error = "INVALID_ENUM";
        break;
      case GL_INVALID_VALUE:
        error = "INVALID_VALUE";
        break;
      case GL_INVALID_OPERATION:
        error = "INVALID_OPERATION";
        break;
      case GL_STACK_OVERFLOW:
        error = "STACK_OVERFLOW";
        break;
      case GL_STACK_UNDERFLOW:
        error = "STACK_UNDERFLOW";
        break;
      case GL_OUT_OF_MEMORY:
        error = "OUT_OF_MEMORY";
        break;
      case GL_INVALID_FRAMEBUFFER_OPERATION:
        error = "INVALID_FRAMEBUFFER_OPERATION";
        break;
    }
    MGE_ERROR("{} | {} ({})", error, file, line);
  }
  MGE_ASSERT(errorCode == GL_NO_ERROR, "Aborting due to opengl errors.");
  return errorCode;
}

std::vector<std::pair<std::string, GLenum>> RenderContext::get_uniforms(GLuint id) {
  GLint count = 0;
  std::vector<std::pair<std::string, GLenum>> uniforms;
  glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &count);

  for (GLint i = 0; i < count; ++i) {
    std::array<GLchar, 256> buffer;
    GLsizei length = 0;
    GLint size = 0;
    GLenum type;
    glGetActiveUniform(id, i, static_cast<GLsizei>(buffer.size()), &length, &size, &type, buffer.data());

    MGE_ASSERT(length >= 1, "Unable to get active uniform {} for program {}", i, id);

    auto names = parse_uniform_names(buffer.data(), length, size);
    for (const auto& name : names) {
      uniforms.push_back({name, type});
    }
  }

  return uniforms;
}

GLuint RenderContext::get_uniform_location(GLuint id, const std::string& name) {
  auto location = glGetUniformLocation(id, name.c_str());
  MGE_ASSERT(location >= 0);
  glCheckError();
  return location;
}

GLuint RenderContext::create_shader(GLenum type) {
  GLuint id = glCreateShader(type);
  glCheckError();
  return id;
}

void RenderContext::destroy_shader(GLuint id) {
  glDeleteShader(id);
  glCheckError();
}

void RenderContext::set_shader_source(GLuint id, const char* source) {
  glShaderSource(id, 1, &source, nullptr);
  glCheckError();
}

void RenderContext::compile_shader(GLuint id) {
  glCompileShader(id);
  MGE_ASSERT(check_shader_compile_erros(id), "Aborting due to shader compile errors: {}", id);
}

GLuint RenderContext::create_shader_program() {
  auto id = glCreateProgram();
  glCheckError();
  return id;
}

void RenderContext::destroy_shader_program(GLuint id) {
  glDeleteProgram(id);
  glCheckError();
}

void RenderContext::bind_shader_program(GLuint program) {
  glUseProgram(program);
  glCheckError();
  m_bound_program = program;
}

void RenderContext::unbind_shader_program(GLuint program) {
  MGE_ASSERT(m_bound_program == program, "Cannot unbind shader program as it is not currently bound: {}", program);

  glUseProgram(0);
  glCheckError();
  m_bound_program = 0;
}

void RenderContext::try_unbind_shader_program(GLuint id) {
  if (m_bound_program == id) {
    unbind_shader_program(id);
  }
}

GLuint RenderContext::get_bound_shader_program() const { return m_bound_program; }

void RenderContext::attach_shader(GLuint program_id, GLuint shader_id) {
  glAttachShader(program_id, shader_id);
  glCheckError();
}

void RenderContext::link_shader_program(GLuint id) {
  glLinkProgram(id);
  MGE_ASSERT(check_shader_program_compile_erros(id), "Aborting due to shader program linking errors: {}", id);
}

GLuint RenderContext::create_buffer() {
  GLuint id;
  glGenBuffers(1, &id);
  glCheckError();
  return id;
}
void RenderContext::destroy_buffer(GLuint id) {
  glDeleteBuffers(1, &id);
  glCheckError();
}

void RenderContext::bind_buffer(GLenum type, GLuint id) {
  glBindBuffer(type, id);
  glCheckError();
  m_bound_buffers[type] = {.id = id, .is_mapped = false};
}

void RenderContext::unbind_buffer(GLenum type, GLuint id) {
  MGE_ASSERT(m_bound_buffers.contains(type) && m_bound_buffers.at(type).id == id,
             "Cannot unbind buffer as it is not currently bound: {}", id);

  glBindBuffer(type, 0);
  glCheckError();
  m_bound_buffers[type] = {.id = 0, .is_mapped = false};
}

void RenderContext::try_unbind_buffer(GLenum type, GLuint id) {
  if (m_bound_buffers.contains(type) && m_bound_buffers.at(type).id == id) {
    unbind_buffer(type, id);
  }
}

GLuint RenderContext::get_bound_buffer(GLenum type) const {
  if (!m_bound_buffers.contains(type)) {
    return 0;
  }

  return m_bound_buffers.at(type).id;
}

void* RenderContext::map_buffer(GLenum type, GLuint id, GLenum access_mode) {
  MGE_ASSERT(m_bound_buffers.contains(type) && m_bound_buffers.at(type).id == id,
             "Cannot map buffer as it is not currently bound: {}", id);

  m_bound_buffers.at(type).is_mapped = true;
  auto ptr = glMapBuffer(type, access_mode);
  glCheckError();
  return ptr;
}

void RenderContext::unmap_buffer(GLenum type, GLuint id) {
  MGE_ASSERT(m_bound_buffers.contains(type) && m_bound_buffers.at(type).id == id && m_bound_buffers.at(type).is_mapped,
             "Cannot unmap buffer as it is not currently mapped: {}", id);

  glUnmapBuffer(type);
  glCheckError();
  m_bound_buffers.at(type).is_mapped = false;
}

GLuint RenderContext::get_mapped_buffer(GLenum type) const {
  if (!m_bound_buffers.contains(type) || !m_bound_buffers.at(type).is_mapped) {
    return 0;
  }

  return m_bound_buffers.at(type).id;
}

void RenderContext::copy_buffer(GLenum type, GLuint id, unsigned int size, const void* data, GLenum usage) {
  MGE_ASSERT(m_bound_buffers.contains(type) && m_bound_buffers.at(type).id == id,
             "Cannot copy to buffer as it is not currently bound: {}", id);

  glBufferData(type, size, data, usage);
  glCheckError();
}

void RenderContext::copy_buffer_subregion(GLenum type, GLuint id, unsigned int offset, unsigned int size,
                                          const void* data) {
  MGE_ASSERT(m_bound_buffers.contains(type) && m_bound_buffers.at(type).id == id,
             "Cannot copy to buffer's subregion as it is not currently bound: {}", id);

  glBufferSubData(type, offset, size, data);
  glCheckError();
}

GLuint RenderContext::create_vertex_array() {
  GLuint id;
  glGenVertexArrays(1, &id);
  glCheckError();
  return id;
}

void RenderContext::destroy_vertex_array(GLuint id) {
  glDeleteVertexArrays(1, &id);
  glCheckError();
}

void RenderContext::bind_vertex_array(GLuint id) {
  glBindVertexArray(id);
  glCheckError();
  m_bound_vertex_array = id;
}

void RenderContext::unbind_vertex_array(GLuint id) {
  MGE_ASSERT(m_bound_vertex_array == id, "Attempted to unbind vertex array that is not currently bound: {}", id);
  glBindVertexArray(0);
  m_bound_vertex_array = 0;
}

void RenderContext::try_unbind_vertex_array(GLuint id) {
  if (m_bound_vertex_array == id) {
    unbind_vertex_array(id);
  }
}

GLuint RenderContext::get_bound_vertex_array() const { return m_bound_vertex_array; }

void RenderContext::add_vertex_array_attribute(GLuint array_id, GLuint attrib_idx, GLuint size, GLenum type,
                                               GLsizei stride, const void* ptr) {
  MGE_ASSERT(m_bound_vertex_array == array_id, "Attempted to add attribute to vertex array that is not bound: {}",
             array_id);
  glEnableVertexAttribArray(attrib_idx);
  glCheckError();
  glVertexAttribPointer(attrib_idx, size, type, GL_FALSE, stride, ptr);
  glCheckError();
}

void RenderContext::add_vertex_array_instanced_attribute(GLuint array_id, GLuint attrib_idx, GLuint size, GLenum type,
                                                         GLsizei stride, const void* ptr, GLuint divisor) {
  add_vertex_array_attribute(array_id, attrib_idx, size, type, stride, ptr);
  glVertexAttribDivisor(attrib_idx, divisor);
  glCheckError();
}

void RenderContext::set_viewport_dims(GLuint minx, GLuint miny, GLuint maxx, GLuint maxy) {
  glViewport(minx, miny, maxx, maxy);
  glCheckError();
}

void RenderContext::set_patch_count(unsigned int patches) { glPatchParameteri(GL_PATCH_VERTICES, patches); }

void RenderContext::draw(GLenum type, unsigned int vertex_count) { glDrawArrays(type, 0, vertex_count); }

void RenderContext::draw_elements(GLenum type, unsigned int element_count) {
  glDrawElements(type, element_count, GL_UNSIGNED_INT, 0);
}

void RenderContext::draw_instanced(GLenum type, unsigned int vertex_count, unsigned int instance_count) {
  glDrawArraysInstanced(type, 0, vertex_count, instance_count);
}

void RenderContext::draw_instanced_elements(GLenum type, unsigned int element_count, unsigned int instance_count) {
  glDrawElementsInstanced(type, element_count, GL_UNSIGNED_INT, 0, instance_count);
}
}  // namespace mge
