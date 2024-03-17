#include "render_context.hh"

namespace mge {
std::unique_ptr<RenderContext> RenderContext::s_instance = nullptr;

RenderContext::RenderContext() {
  MGE_INFO("Render context created");
  s_instance->init();
}

RenderContext::~RenderContext() {
  if (s_instance) {
    s_instance->terminate();
  }
}

RenderContext& RenderContext::create() {
  if (s_instance) {
    throw std::runtime_error("Render context already exists!");
  }

  s_instance = std::unique_ptr<RenderContext>(new RenderContext());
  return *s_instance;
}

void RenderContext::init() {
  if (!gladLoadGL(glfwGetProcAddress)) {
    throw std::runtime_error("Failed to initialize GLAD");
  }

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
  return errorCode;
}

}  // namespace mge
