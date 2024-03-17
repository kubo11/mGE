#ifndef MGE_RENDER_API_SHADER
#define MGE_RENDER_API_SHADER

#include "../mgepch.hh"
#include "../render_context.hh"

namespace mge {
class Shader {
 public:
  Shader();
  Shader(const fs::path& vertex_path, const fs::path& fragment_path);
  ~Shader();

  inline const unsigned int get_id() const { return m_id; }
  inline const fs::path& get_path() const { return m_path; }
  inline bool operator==(const Shader& s) { return m_id == s.m_id; }

  void use() const;
  bool is_used() const;
  void unuse() const;

  void set_uniform(const std::string& name, bool value);
  void set_uniform(const std::string& name, int value);
  void set_uniform(const std::string& name, float value);
  void set_uniform(const std::string& name, const glm::vec3& value);
  void set_uniform(const std::string& name, const glm::mat4& value);

 private:
  static GLuint s_current_shader_id;
  unsigned int m_id = 0;
  const fs::path m_path;

  static void check_shader_compile_erros(unsigned int shader_id);
  static void check_program_compile_erros(unsigned int program_id);
  static std::string read_shader_code(const fs::path& path);

  GLint get_uniform_id(const std::string& name);
};
}  // namespace mge

#endif  // MGE_RENDER_API_SHADER