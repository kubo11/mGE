#ifndef MGE_SHADER_SYSTEM_HH
#define MGE_SHADER_SYSTEM_HH

#include "mgepch.hh"

#include "render_api/shader.hh"

namespace mge {
class ShaderSystem {
 public:
  ~ShaderSystem();

  static ShaderSystem& create();
  static ShaderSystem& get_instance();
  static Shader& acquire(const fs::path& shader);
  static void unload(const fs::path& path);
  static void unload(Shader& shader);

  void terminate();

 private:
  static std::unique_ptr<ShaderSystem> s_instance;
  std::unordered_map<fs::path, std::unique_ptr<Shader>> m_shaders;

  ShaderSystem();

  void init();
  static Shader& load(const fs::path& shader);
};
}  // namespace mge

#endif  // MGE_SHADER_SYSTEM_HH