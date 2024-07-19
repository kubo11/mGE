#ifndef MGE_RENDERER_COMPONENTS_RENDERABLE_COMPONENT_HH
#define MGE_RENDERER_COMPONENTS_RENDERABLE_COMPONENT_HH

#include "../../../mgepch.hh"

#include "../../../render_api/shader.hh"
#include "../../../render_api/vertex_array.hh"
#include "../../../shader_system.hh"

#define RENDER_MODE_SIZE 2

namespace mge {
enum class RenderMode : int { SOLID = 1 << 0, WIREFRAME = 1 << 1 };

inline GLenum render_mode_to_gl(RenderMode mode) {
  switch (mode) {
    case RenderMode::WIREFRAME:
      return GL_LINE;
    case RenderMode::SOLID:
      return GL_TRIANGLES;

    default:
      return GL_NONE;
  }
}

inline std::string to_string(RenderMode mode) {
  switch (mode) {
    case RenderMode::SOLID:
      return "solid";
    case RenderMode::WIREFRAME:
      return "wireframe";

    default:
      return "invalid";
  }
};

template <class T>
class RenderPipeline;

template <class T>
using RenderPipelineMap = std::map<RenderMode, std::reference_wrapper<RenderPipeline<T>>>;

template <class T>
struct RenderableComponent {
  RenderableComponent(RenderPipelineMap<T> render_pipelines, RenderMode mode,
                      std::unique_ptr<VertexArray<T>> vertex_array, glm::vec3 color = {0.0f, 0.0f, 0.0f})
      : m_render_pipelines(std::move(render_pipelines)),
        m_vertex_array(std::move(vertex_array)),
        m_color(color),
        m_render_mode(mode),
        m_enabled(true) {
    get_render_pipeline().add_renderable(*this);
  }

  inline VertexArray<T>& get_vertex_array() { return *m_vertex_array; }
  inline glm::vec3 get_color() const { return m_color; }
  inline RenderMode get_render_mode() const { return m_render_mode; }
  inline RenderPipeline<T>& get_render_pipeline() { return m_render_pipelines.at(m_render_mode); }
  inline void set_render_mode(RenderMode mode) {
    MGE_ASSERT(m_render_pipelines.contains(mode),
               "Attempted to set render mode for renderable component that is not avaliable: {}", to_string(mode));
    get_render_mode().remove_renderable(*this);
    m_render_mode = mode;
    get_render_mode().add_renderable(*this);
  }
  inline void set_color(glm::vec3 color) { m_color = color; }
  inline void enable() { m_enabled = true; }
  inline void disable() { m_enabled = false; }
  inline bool is_enabled() const { return m_enabled; }

  inline void draw() {
    if (!m_enabled) return;
    m_vertex_array->bind();
    // get_render_pipeline().dynamic_uniform_update();
    if (m_vertex_array->has_element_buffer()) {
      RenderContext::get_instance().draw_elements(render_mode_to_gl(m_render_mode), m_vertex_array->get_draw_size());
    } else {
      RenderContext::get_instance().draw(render_mode_to_gl(m_render_mode), m_vertex_array->get_draw_size());
    }
    m_vertex_array->unbind();
  }

 private:
  RenderPipelineMap<T> m_render_pipelines;
  std::unique_ptr<VertexArray<T>> m_vertex_array;
  glm::vec3 m_color;
  RenderMode m_render_mode;
  bool m_enabled;
};

template <class T, class N>
class InstancedRenderableComponent {};
}  // namespace mge

#endif  // MGE_RENDERER_COMPONENTS_RENDERABLE_COMPONENT_HH