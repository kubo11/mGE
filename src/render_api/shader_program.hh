#ifndef MGE_RENDER_API_SHADER_PROGRAM
#define MGE_RENDER_API_SHADER_PROGRAM

#include "mgepch.hh"

#include "render_api/shader.hh"
#include "render_api/uniform.hh"
#include "render_api/uniform_types.hh"

namespace mge {
using UniformMap = std::unordered_map<std::string, std::unique_ptr<Uniform>>;
using ShaderMap = std::unordered_map<Shader::Type, std::unique_ptr<Shader>>;

class ShaderProgram {
 public:
  ShaderProgram();
  ~ShaderProgram();

  PREVENT_COPY(ShaderProgram);

  void attach(std::unique_ptr<Shader> shader);
  bool link();
  void bind();

  inline const GLuint get_id() const { return m_id; }
  inline Shader &get_shader(Shader::Type type) { return *m_shaders.at(type); }

  inline bool has_uniform(const std::string &name) const {
    return m_uniforms.contains(name);
  }

  template <typename T>
  inline void set_uniform_value(const std::string &name, const T &value) {
    if (!m_uniforms.contains(name)) {
      MGE_ASSERT(false, "Uniform {} doesn't exist", name.c_str());
      return;
    }

    m_uniforms.at(name)->set_value(value);
  }

 private:
  GLuint m_id;
  ShaderMap m_shaders;
  UniformMap m_uniforms;

  void release();
  void load_uniforms();
  void load_uniforms_at_index(GLuint index);
};
}  // namespace mge

#endif  // MGE_RENDER_API_SHADER_PROGRAM