#ifndef MGE_RENDER_API_SHADER
#define MGE_RENDER_API_SHADER

#include "../mgepch.hh"

namespace mge {
class Shader {
 public:
  Shader(const fs::path& vertex_path, const fs::path& fragment_path);

  inline const unsigned int get_id() const { return id; }

  void use();

  void set_uniform(const std::string& name, bool value);
  void set_uniform(const std::string& name, int value);
  void set_uniform(const std::string& name, float value);

 private:
  unsigned int id;

  static void check_shader_compile_erros(unsigned int shader_id);
  static void check_program_compile_erros(unsigned int program_id);
  static std::string read_shader_code(const fs::path& path);
};
}  // namespace mge

#endif  // MGE_RENDER_API_SHADER