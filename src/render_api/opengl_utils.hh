#ifndef MGE_RENDER_API_OPENGL_UTILS_HH
#define MGE_RENDER_API_OPENGL_UTILS_HH

#include "../mgepch.hh"

namespace mge {
inline size_t gl_sizeof_type(GLenum type) {
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

inline GLenum glCheckError_(const char *file, int line) {
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
#define glCheckError() glCheckError_(__FILE__, __LINE__)
}  // namespace mge

#endif  // MGE_RENDER_API_OPENGL_UTILS_HH