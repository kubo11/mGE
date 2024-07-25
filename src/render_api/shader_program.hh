#ifndef MGE_RENDER_API_SHADER_PROGRAM
#define MGE_RENDER_API_SHADER_PROGRAM

#include "../mgepch.hh"

#include "shader.hh"
#include "uniform.hh"
#include "uniform_types.hh"

namespace mge {
using UniformMap = std::unordered_map<std::string, std::unique_ptr<Uniform>>;
using ShaderMap = std::unordered_map<Shader::Type, std::unique_ptr<Shader>>;

class ShaderProgram {
 public:
  ShaderProgram();
  ~ShaderProgram();

  PREVENT_COPY(ShaderProgram);

  void attach(std::unique_ptr<Shader> shader);
  void link();
  void bind();
  void unbind();
  void try_unbind();

  inline const GLuint get_id() const { return m_id; }
  inline bool has_shader(Shader::Type type) const { return m_shaders.contains(type); }
  inline Shader &get_shader(Shader::Type type) { return *m_shaders.at(type); }

  inline bool has_uniform(const std::string &name) const { return m_uniforms.contains(name); }

  template <typename T>
  inline void set_uniform_value(const std::string &name, const T &value) {
    if (!m_uniforms.contains(name)) {
      MGE_ASSERT(false, "Uniform {} doesn't exist", name.c_str());
      return;
    }

    m_uniforms.at(name)->set_value(value);
  }

  template <typename T>
  inline void set_and_commit_uniform_value(const std::string &name, const T &value) {
    if (!m_uniforms.contains(name)) {
      MGE_ASSERT(false, "Uniform {} doesn't exist", name.c_str());
      return;
    }

    m_uniforms.at(name)->set_value(value);
    m_uniforms.at(name)->commit();
  }

 private:
  GLuint m_id;
  ShaderMap m_shaders;
  UniformMap m_uniforms;

  void release();
  void load_uniforms();
};
}  // namespace mge

#endif  // MGE_RENDER_API_SHADER_PROGRAM