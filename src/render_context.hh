#ifndef MGE_RENDER_CONTEXT_HH
#define MGE_RENDER_CONTEXT_HH

#include "mgepch.hh"

class ShaderProgram;

namespace mge {
class RenderContext {
 public:
  ~RenderContext();

  static std::shared_ptr<RenderContext> create();
  void terminate();

  static size_t glSizeofType(GLenum type);
  static GLenum glCheckError_(const char* file, int line);

  inline static RenderContext& get() { return *s_instance; }

  void bind_shader_program(GLuint program);

 private:
  static std::shared_ptr<RenderContext> s_instance;
  GLuint m_program;

  RenderContext();
  void init();
};

#define glCheckError() RenderContext::glCheckError_(__FILE__, __LINE__)
}  // namespace mge

#endif  // MGE_RENDER_CONTEXT_HH