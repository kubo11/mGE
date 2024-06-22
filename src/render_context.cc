#include "render_context.hh"

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
  MGE_ASSERT(res == 0, "Failed to initialize GLAD");

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glEnable(GL_MULTISAMPLE);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  MGE_INFO("Render context initialized");
}

void RenderContext::terminate() { MGE_INFO("Render context terminated"); }

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

GLuint RenderContext::get_bound_shader_program() const { return m_bound_program; }

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

void RenderContext::copy_buffer(GLenum type, GLuint id, unsigned int size, void* data, GLenum usage) {
  MGE_ASSERT(m_bound_buffers.contains(type) && m_bound_buffers.at(type).id == id,
             "Cannot copy to buffer as it is not currently bound: {}", id);

  glBufferData(type, size, data, usage);
  glCheckError();
}

void RenderContext::copy_buffer_subregion(GLenum type, GLuint id, unsigned int offset, unsigned int size, void* data) {
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

}  // namespace mge
