#ifndef MGE_RENDER_API_SHADER
#define MGE_RENDER_API_SHADER

#include "../mgepch.hh"
#include "../render_context.hh"

namespace mge {
class Shader {
 public:
  enum class Type { VERTEX, TESSELLATION_CONTROL, TESSELLATION_EVALUATION, GEOMETRY, FRAGMENT, COMPUTE };

  Shader(Type type);
  ~Shader();

  PREVENT_COPY(Shader);

  bool compile(const char* source);

  inline const GLuint get_id() const { return m_id; }

  inline const Type get_type() const { return m_type; }

  static GLenum to_gl_shader_type(Type type);

 private:
  GLuint m_id;
  Type m_type;

  void release();
};
}  // namespace mge

#endif  // MGE_RENDER_API_SHADER