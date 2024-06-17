#ifndef MGE_RENDERER_COMPONENTS_RENDERABLE_COMPONENT_HH
#define MGE_RENDERER_COMPONENTS_RENDERABLE_COMPONENT_HH

#include "../../../mgepch.hh"

#include "../../../render_api/shader.hh"
#include "../../../render_api/vertex_array.hh"
#include "../../../shader_system.hh"

#define RENDER_MODE_SIZE 2

namespace mge {
enum class RenderMode : int { SURFACE = 1 << 0, WIREFRAME = 1 << 1, PATCHES = 1 << 2 };

inline std::string to_string(RenderMode mode) {
  switch (mode) {
    case RenderMode::SURFACE:
      return "surface";
    case RenderMode::WIREFRAME:
      return "wireframe";

    default:
      return "invalid";
  }
};

template <class T>
struct RenderableComponent {
  RenderableComponent(std::shared_ptr<Shader> shader, std::unique_ptr<VertexArray<T>> vertex_array = nullptr,
                      glm::vec3 color = {0.0f, 0.0f, 0.0f})
      : m_shader(shader),
        m_vertex_array(std::move(vertex_array)),
        m_color(color),
        m_render_mode(RenderMode::WIREFRAME),
        m_enabled(true) {}
  RenderableComponent(RenderableComponent<T>&& other)
      : m_shader(std::move(other.m_shader)),
        m_vertex_array(std::move(other.m_vertex_array)),
        m_color(std::move(other.m_color)),
        m_render_mode(std::move(other.m_render_mode)),
        m_enabled(std::move(other.m_enabled)) {}
  inline RenderableComponent& operator=(RenderableComponent&& other) {
    m_shader = std::move(other.m_shader);
    m_vertex_array.reset(other.m_vertex_array.release());
    m_color = std::move(other.m_color);
    m_render_mode = std::move(other.m_render_mode);
    m_enabled = std::move(other.m_enabled);
    return *this;
  }

  inline std::shared_ptr<Shader> get_shader() const { return m_shader; }
  inline VertexArray<T>& get_vertex_array() const { return *m_vertex_array; }
  inline glm::vec3 get_color() const { return m_color; }
  inline RenderMode get_render_mode() const { return m_render_mode; }
  inline void set_render_mode(RenderMode mode) { m_render_mode = mode; }
  inline void set_color(glm::vec3 color) { m_color = color; }
  inline void enable() { m_enabled = true; }
  inline void disable() { m_enabled = false; }
  inline bool is_enabled() const { return m_enabled; }

 private:
  std::shared_ptr<Shader> m_shader;
  std::unique_ptr<VertexArray<T>> m_vertex_array;
  glm::vec3 m_color;
  RenderMode m_render_mode;
  bool m_enabled;
};
}  // namespace mge

#endif  // MGE_RENDERER_COMPONENTS_RENDERABLE_COMPONENT_HH