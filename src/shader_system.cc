#include "shader_system.hh"

namespace mge {
std::unique_ptr<ShaderSystem> ShaderSystem::s_instance = nullptr;
const std::unordered_map<GLenum, std::string>
    ShaderSystem::s_shader_extensions = {
        {GL_VERTEX_SHADER, ".vert"},          {GL_TESS_CONTROL_SHADER, ".tesc"},
        {GL_TESS_EVALUATION_SHADER, ".tese"}, {GL_GEOMETRY_SHADER, ".geom"},
        {GL_FRAGMENT_SHADER, ".frag"},        {GL_COMPUTE_SHADER, ".comp"},
};

ShaderSystem& ShaderSystem::create() {
  s_instance = std::unique_ptr<ShaderSystem>(new ShaderSystem());
  MGE_INFO("Shader system created");
  s_instance->init();

  return *s_instance;
}

ShaderSystem& ShaderSystem::get_instance() { return *ShaderSystem::s_instance; }

std::shared_ptr<Shader> ShaderSystem::acquire(const fs::path& shader) {
  if (s_instance->m_shaders.contains(shader)) {
    return s_instance->m_shaders.at(shader);
  }

  return load(shader);
}

void ShaderSystem::unload(const std::shared_ptr<Shader>& shader) {
  unload(shader->get_path());
}

void ShaderSystem::unload(const fs::path& path) {
  if (s_instance->m_shaders.contains(path)) {
    s_instance->m_shaders.erase(path);
  }
}

void ShaderSystem::terminate() {
  std::unordered_map<fs::path, std::shared_ptr<Shader>> empty_map;
  m_shaders.swap(empty_map);
  s_instance = nullptr;

  MGE_INFO("Shader system terminated");
}

ShaderSystem::ShaderSystem() {}

ShaderSystem::~ShaderSystem() {
  if (s_instance) {
    terminate();
  }
}

void ShaderSystem::init() { MGE_INFO("Shader system initialized"); }

std::shared_ptr<Shader> ShaderSystem::load(const fs::path& shader) {
  std::unordered_map<GLenum, fs::path> shaders;
  for (const auto& [type, extension] : s_shader_extensions) {
    auto path = fs::path(shader).replace_extension(extension);
    if (fs::exists(path)) {
      shaders.emplace(type, path);
    }
  }

  s_instance->m_shaders.insert({shader, std::make_shared<Shader>(shaders)});

  return s_instance->m_shaders.at(shader);
}

}  // namespace mge
