#ifndef MGE_SHADER_SYSTEM_HH
#define MGE_SHADER_SYSTEM_HH

#include "mgepch.hh"

#include "render_api/shader.hh"
#include "render_api/shader_program.hh"

namespace mge {
class ShaderSystem {
 public:
  ~ShaderSystem();

  static ShaderSystem& create();
  static ShaderSystem& get_instance();
  static std::shared_ptr<ShaderProgram> acquire(const fs::path& shader);
  static void unload(const fs::path& path);

  void terminate();

 private:
  static std::unique_ptr<ShaderSystem> s_instance;
  static const std::unordered_map<Shader::Type, std::string>
      s_shader_extensions;
  std::unordered_map<fs::path, std::shared_ptr<ShaderProgram>> m_shaders;

  ShaderSystem();

  void init();
  static std::shared_ptr<ShaderProgram> load(const fs::path& shader_path);
  static std::string read_shader_code(const fs::path& path);
};
}  // namespace mge

#endif  // MGE_SHADER_SYSTEM_HH