#include "shader_system.hh"

namespace mge {
std::shared_ptr<ShaderSystem> ShaderSystem::s_instance = nullptr;
const std::unordered_map<Shader::Type, std::string> ShaderSystem::s_shader_extensions = {
    {Shader::Type::VERTEX, ".vert"},
    {Shader::Type::TESSELLATION_CONTROL, ".tesc"},
    {Shader::Type::TESSELLATION_EVALUATION, ".tese"},
    {Shader::Type::GEOMETRY, ".geom"},
    {Shader::Type::FRAGMENT, ".frag"},
    {Shader::Type::COMPUTE, ".comp"},
};

std::shared_ptr<ShaderSystem> ShaderSystem::create() {
  s_instance = std::shared_ptr<ShaderSystem>(new ShaderSystem());
  MGE_INFO("Shader system created");
  s_instance->init();

  return s_instance;
}

ShaderSystem& ShaderSystem::get_instance() { return *ShaderSystem::s_instance; }

std::shared_ptr<ShaderProgram> ShaderSystem::acquire(const fs::path& shader) {
  if (s_instance->m_shaders.contains(shader)) {
    return s_instance->m_shaders.at(shader);
  }

  return load(shader);
}

void ShaderSystem::unload(const fs::path& path) {
  if (s_instance->m_shaders.contains(path)) {
    s_instance->m_shaders.erase(path);
  }
}

void ShaderSystem::terminate() {
  std::unordered_map<fs::path, std::shared_ptr<ShaderProgram>> empty_map;
  s_instance->m_shaders.swap(empty_map);
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

std::shared_ptr<ShaderProgram> ShaderSystem::load(const fs::path& shader_path) {
  auto shader_program = std::make_shared<ShaderProgram>();
  for (const auto& [type, extension] : s_shader_extensions) {
    auto path = fs::path(shader_path).replace_extension(extension);
    if (fs::exists(path)) {
      auto shader = std::make_unique<Shader>(type);
      shader->compile(read_shader_code(path).c_str());
      shader_program->attach(std::move(shader));
    }
  }

  shader_program->link();
  s_instance->m_shaders.insert({shader_path, shader_program});

  return s_instance->m_shaders.at(shader_path);
}

std::string ShaderSystem::read_shader_code(const fs::path& path) {
  std::string code;
  std::ifstream file;

  file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    file.open(path);
    std::stringstream stream;
    stream << file.rdbuf();
    file.close();
    code = stream.str();
  } catch (std::ifstream::failure& e) {
    MGE_ERROR("Could not read shader file: {}", e.what());
  }

  return code;
}

}  // namespace mge
