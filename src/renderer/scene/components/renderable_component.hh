#ifndef MGE_RENDERER_COMPONENTS_RENDERABLE_COMPONENT_HH
#define MGE_RENDERER_COMPONENTS_RENDERABLE_COMPONENT_HH

#include "../../../mgepch.hh"

#include "../../../render_api/shader.hh"
#include "../../../render_api/vertex_array.hh"
#include "../../../shader_system.hh"

#define RENDER_MODE_SIZE 2

namespace mge {
enum class RenderMode : int { SOLID = 1 << 0, WIREFRAME = 1 << 1 };

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

template <class T, class N>
class InstancedRenderPipeline;

enum class DrawPrimitiveType;
GLenum draw_primitive_type_to_gl(DrawPrimitiveType type);

template <class T>
using RenderPipelineMap = std::map<RenderMode, std::reference_wrapper<RenderPipeline<T>>>;

template <class T>
class RenderableComponent;

template <class T>
bool operator==(const std::reference_wrapper<RenderableComponent<T>>& a,
                const std::reference_wrapper<RenderableComponent<T>>& b) {
  return a.get() == b.get();
}

template <class T>
struct RenderableComponent {
  RenderableComponent(
      RenderPipelineMap<T> render_pipelines, RenderMode mode, std::unique_ptr<VertexArray<T>> vertex_array,
      const std::function<void(RenderPipeline<T>&)>& dynamic_update_func = [](RenderPipeline<T>&) {})
      : m_render_pipelines(std::move(render_pipelines)),
        m_vertex_array(std::move(vertex_array)),
        m_render_mode(mode),
        m_enabled(true),
        m_dynamic_update_func(dynamic_update_func) {
    get_render_pipeline().add_renderable(*this);
  }

  ~RenderableComponent() { get_render_pipeline().remove_renderable(*this); }

  friend bool operator== <T>(const std::reference_wrapper<RenderableComponent<T>>& a,
                             const std::reference_wrapper<RenderableComponent<T>>& b);

  inline bool operator==(const RenderableComponent<T>& other) const {
    return m_enabled == other.m_enabled && m_render_mode == other.m_render_mode &&
           m_vertex_array == other.m_vertex_array;
  }
  inline VertexArray<T>& get_vertex_array() { return *m_vertex_array; }
  inline RenderMode get_render_mode() const { return m_render_mode; }
  inline std::vector<RenderMode> get_registered_render_modes() const {
    auto kv = std::views::keys(m_render_pipelines);
    return {kv.begin(), kv.end()};
  }
  inline RenderPipeline<T>& get_render_pipeline() { return m_render_pipelines.at(m_render_mode); }
  inline void set_render_mode(RenderMode mode) {
    MGE_ASSERT(m_render_pipelines.contains(mode),
               "Attempted to set render mode for renderable component that is not avaliable: {}", to_string(mode));
    m_render_pipelines.at(get_render_mode()).get().remove_renderable(*this);
    m_render_mode = mode;
    m_render_pipelines.at(get_render_mode()).get().add_renderable(*this);
  }
  inline void enable() { m_enabled = true; }
  inline void disable() { m_enabled = false; }
  inline bool is_enabled() const { return m_enabled; }

  inline void draw() {
    if (!m_enabled) return;
    m_vertex_array->bind();
    m_dynamic_update_func(get_render_pipeline());
    if (m_vertex_array->has_element_buffer()) {
      RenderContext::get_instance().draw_elements(
          draw_primitive_type_to_gl(get_render_pipeline().get_draw_primitive_type()), m_vertex_array->get_draw_size());
    } else {
      RenderContext::get_instance().draw(draw_primitive_type_to_gl(get_render_pipeline().get_draw_primitive_type()),
                                         m_vertex_array->get_draw_size());
    }
    m_vertex_array->unbind();
  }

 private:
  RenderPipelineMap<T> m_render_pipelines;
  std::unique_ptr<VertexArray<T>> m_vertex_array;
  RenderMode m_render_mode;
  bool m_enabled;
  std::function<void(RenderPipeline<T>&)> m_dynamic_update_func;
};

template <class T, class N>
using InstancedRenderPipelineMap = std::map<RenderMode, std::reference_wrapper<InstancedRenderPipeline<T, N>>>;

template <class T, class N>
struct InstancedRenderableComponent;

template <class T, class N>
bool operator==(const std::reference_wrapper<InstancedRenderableComponent<T, N>>& a,
                const std::reference_wrapper<InstancedRenderableComponent<T, N>>& b) {
  return a.get() == b.get();
}

template <class T, class N>
struct InstancedRenderableComponent {
  InstancedRenderableComponent(InstancedRenderPipelineMap<T, N> render_pipelines, RenderMode mode, N instance_data)
      : m_render_pipelines(std::move(render_pipelines)),
        m_instance_data(std::move(instance_data)),
        m_render_mode(mode),
        m_enabled(true) {
    get_render_pipeline().add_renderable(*this);
  }

  ~InstancedRenderableComponent() { get_render_pipeline().remove_renderable(*this); }

  PREVENT_COPY(InstancedRenderableComponent);

  friend bool operator== <T>(const std::reference_wrapper<InstancedRenderableComponent<T, N>>& a,
                             const std::reference_wrapper<InstancedRenderableComponent<T, N>>& b);

  inline bool operator==(const InstancedRenderableComponent<T, N>& other) const {
    return m_instance_data == other.m_instance_data && m_enabled == other.m_enabled &&
           m_render_mode == other.m_render_mode && m_render_pipelines == other.m_render_pipelines;
  }

  inline const N& get_instance_data() { return m_instance_data; }
  inline void set_instance_data(const N& data) {
    m_instance_data = data;
    if (is_enabled() && !m_dirty) get_render_pipeline().schedule_instance_data_update(*this);
    m_dirty = true;
  }
  inline RenderMode get_render_mode() const { return m_render_mode; }
  inline InstancedRenderPipeline<T, N>& get_render_pipeline() { return m_render_pipelines.at(m_render_mode); }
  inline void set_render_mode(RenderMode mode) {
    MGE_ASSERT(m_render_pipelines.contains(mode),
               "Attempted to set render mode for renderable component that is not avaliable: {}", to_string(mode));
    get_render_mode().remove_renderable(*this);
    m_render_mode = mode;
    get_render_mode().add_renderable(*this);
    if (m_dirty) get_render_pipeline().schedule_instance_data_update(*this);
  }
  inline void enable() {
    if (!m_enabled) {
      m_enabled = true;
      m_render_pipelines.at(get_render_mode()).get().add_renderable(*this);
      if (m_dirty) get_render_pipeline().schedule_instance_data_update(*this);
    }
  }
  inline void disable() {
    if (m_enabled) {
      m_enabled = false;
      m_render_pipelines.at(get_render_mode()).get().remove_renderable(*this);
    }
  }
  inline bool is_enabled() const { return m_enabled; }

  bool is_dirty() const { return m_dirty; }
  void set_clean() { m_dirty = false; }

 private:
  InstancedRenderPipelineMap<T, N> m_render_pipelines;
  N m_instance_data;
  RenderMode m_render_mode;
  bool m_enabled;
  bool m_dirty;
};
}  // namespace mge

#endif  // MGE_RENDERER_COMPONENTS_RENDERABLE_COMPONENT_HH