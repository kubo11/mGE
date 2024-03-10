#include "shader_system.hh"

namespace mge {
std::unique_ptr<ShaderSystem> ShaderSystem::s_instance = nullptr;

ShaderSystem& ShaderSystem::create() {
  s_instance = std::unique_ptr<ShaderSystem>(new ShaderSystem());
  MGE_INFO("Shader system created");
  s_instance->init();

  return *s_instance;
}

ShaderSystem& ShaderSystem::get_instance() { return *ShaderSystem::s_instance; }

Shader& ShaderSystem::acquire(const fs::path& shader) {
  if (s_instance->m_shaders.contains(shader)) {
    return *s_instance->m_shaders.at(shader);
  }

  return load(shader);
}

void ShaderSystem::unload(Shader& shader) { unload(shader.get_path()); }

void ShaderSystem::unload(const fs::path& path) {
  if (s_instance->m_shaders.contains(path)) {
    s_instance->m_shaders.erase(path);
  }
}

void ShaderSystem::terminate() {
  std::unordered_map<fs::path, std::unique_ptr<Shader>> empty_map;
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

Shader& ShaderSystem::load(const fs::path& shader) {
  auto vert_path = fs::path(shader).replace_extension(".vert");
  auto frag_path = fs::path(shader).replace_extension(".frag");

  s_instance->m_shaders.insert(
      {shader, std::make_unique<Shader>(vert_path, frag_path)});

  return *s_instance->m_shaders.at(shader);
}

}  // namespace mge
