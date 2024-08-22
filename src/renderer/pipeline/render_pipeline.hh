#ifndef MGE_RENDERER_PIPELINE_RENDER_PIPELINE
#define MGE_RENDERER_PIPELINE_RENDER_PIPELINE

#include "../../mgepch.hh"

#include "../../render_api/shader_program.hh"
#include "../../render_api/vertex_array.hh"
#include "../scene/components/renderable_component.hh"
#include "render_pipeline_action.hh"

namespace mge {
enum class DrawPrimitiveType { POINT, LINE, LINE_STRIP, TRIANGLE, TRIANGLE_STRIP, PATCH };

inline GLenum draw_primitive_type_to_gl(DrawPrimitiveType type) {
  switch (type) {
    case DrawPrimitiveType::POINT:
      return GL_POINTS;
    case DrawPrimitiveType::LINE:
      return GL_LINES;
    case DrawPrimitiveType::LINE_STRIP:
      return GL_LINE_STRIP;
    case DrawPrimitiveType::TRIANGLE:
      return GL_TRIANGLES;
    case DrawPrimitiveType::TRIANGLE_STRIP:
      return GL_TRIANGLE_STRIP;
    case DrawPrimitiveType::PATCH:
      return GL_PATCHES;

    default:
      return GL_NONE;
  }
}

template <class T>
class BaseRenderPipeline {
 public:
  virtual ~BaseRenderPipeline() = default;

  PREVENT_COPY(BaseRenderPipeline);

  virtual void run() = 0;

  template <class N>
  void dynamic_uniform_update(const std::string& uniform_name, const std::function<N(void)>& update_func) {
    MGE_ASSERT(m_shader_program.has_uniform(uniform_name), "Attempted to update unknown uniform: {}", uniform_name);
    m_shader_program.set_uniform_value(uniform_name, update_func());
  }

  template <class N>
  void dynamic_uniform_update_and_commit(const std::string& uniform_name, const std::function<N(void)>& update_func) {
    MGE_ASSERT(m_shader_program.has_uniform(uniform_name), "Attempted to update unknown uniform: {}", uniform_name);
    m_shader_program.set_and_commit_uniform_value(uniform_name, update_func());
  }

  void dynamic_buffer_update(Buffer<T>& buffer, const std::function<std::pair<T*, unsigned int>(void)>& update_func) {
    bool buffer_found = false;
    for (auto& action : m_buffer_actions) {
      auto& buffer_action = dynamic_cast<BufferAction<T>>(action);
      if (buffer_action.m_buffer == buffer) {
        buffer_found = true;
        break;
      }
    }
    MGE_ASSERT(buffer_found, "Attempted to dynamically update buffer that is not part of render pipeline");
    auto [data, size] = update_func();
    buffer.copy(size, data);
  }

  void dynamic_buffer_update(Buffer<T>& buffer, const std::function<std::vector<T>(void)>& update_func) {
    dynamic_buffer_update(buffer, [&update_func]() {
      auto data = update_func();
      return std::pair<T*, unsigned int>{data.data(), data.size()};
    });
  }

  DrawPrimitiveType get_draw_primitive_type() const { return m_draw_primitive_type; }

 protected:
  BaseRenderPipeline(ShaderProgram& shader_program, DrawPrimitiveType type)
      : m_shader_program{shader_program}, m_draw_primitive_type(type) {}

  ShaderProgram& m_shader_program;
  std::vector<std::unique_ptr<RenderPipelineAction>> m_uniform_actions;
  std::vector<std::unique_ptr<RenderPipelineAction>> m_buffer_actions;
  unsigned int m_patch_count = 0;
  DrawPrimitiveType m_draw_primitive_type;

  friend class RenderPipelineBuilder;
};

template <class T>
class RenderPipeline;

template <class T>
bool operator==(const std::reference_wrapper<RenderPipeline<T>>& a,
                const std::reference_wrapper<RenderPipeline<T>>& b) {
  return a.get().m_renderables == b.get().m_renderables;
}

template <class T>
class RenderPipeline : public BaseRenderPipeline<T> {
 public:
  PREVENT_COPY(RenderPipeline);

  friend bool operator==
      <T>(const std::reference_wrapper<RenderPipeline<T>>& a, const std::reference_wrapper<RenderPipeline<T>>& b);

  virtual void run() override {
    this->m_shader_program.bind();
    for (auto& action : this->m_uniform_actions) {
      action->run();
    }
    for (auto& action : this->m_buffer_actions) {
      action->run();
    }

    if (this->m_patch_count > 0) {
      RenderContext::get_instance().set_patch_count(this->m_patch_count);
    }

    for (auto& renderable : m_renderables) {
      renderable.get().draw();
    }

    for (auto& action : this->m_buffer_actions | std::views::reverse) {
      action->cleanup();
    }
    for (auto& action : this->m_uniform_actions | std::views::reverse) {
      action->cleanup();
    }
    this->m_shader_program.unbind();
  }

  void add_renderable(RenderableComponent<T>& renderable) { m_renderables.push_back(renderable); }

  void remove_renderable(RenderableComponent<T>& renderable) {
    vector_remove_if<RenderableComponent<T>>(
        m_renderables, [&renderable](std::reference_wrapper<RenderableComponent<T>> renderable_ref) -> bool {
          return renderable_ref.get() == renderable;
        });
  }

 protected:
  RenderPipeline(ShaderProgram& shader_program, DrawPrimitiveType type) : BaseRenderPipeline<T>(shader_program, type) {}

  std::vector<std::reference_wrapper<RenderableComponent<T>>> m_renderables;

  friend class RenderPipelineBuilder;
};

template <class T, class N>
class InstancedRenderPipeline;

template <class T, class N>
bool operator==(const std::reference_wrapper<InstancedRenderPipeline<T, N>>& a,
                const std::reference_wrapper<InstancedRenderPipeline<T, N>>& b) {
  return a.get().m_vertex_array == b.get().m_vertex_array;
}

template <class T, class N>
class InstancedRenderPipeline : public BaseRenderPipeline<T> {
 public:
  PREVENT_COPY(InstancedRenderPipeline);

  friend bool operator== <T, N>(const std::reference_wrapper<InstancedRenderPipeline<T, N>>& a,
                                const std::reference_wrapper<InstancedRenderPipeline<T, N>>& b);

  virtual void run() override {
    if (m_do_full_instance_data_update) {
      update_instance_buffer();
      m_do_full_instance_data_update = false;
    }
    if (!m_renderables_to_update.empty()) {
      flush_scheduled_instance_data_updates();
    }

    this->m_shader_program.bind();
    m_vertex_array->bind();
    for (auto& action : this->m_uniform_actions) {
      action->run();
    }
    for (auto& action : this->m_buffer_actions) {
      action->run();
    }

    if (this->m_patch_count > 0) {
      RenderContext::get_instance().set_patch_count(this->m_patch_count);
    }

    if (m_vertex_array->has_element_buffer()) {
      RenderContext::get_instance().draw_instanced_elements(draw_primitive_type_to_gl(this->m_draw_primitive_type),
                                                            m_vertex_array->get_draw_size(),
                                                            m_vertex_array->get_instance_buffer().get_count());
    } else {
      RenderContext::get_instance().draw_instanced(draw_primitive_type_to_gl(this->m_draw_primitive_type),
                                                   m_vertex_array->get_draw_size(),
                                                   m_vertex_array->get_instance_buffer().get_count());
    }

    for (auto& action : this->m_buffer_actions | std::views::reverse) {
      action->cleanup();
    }
    for (auto& action : this->m_uniform_actions | std::views::reverse) {
      action->cleanup();
    }
    m_vertex_array->unbind();
    this->m_shader_program.unbind();
  }

  void add_renderable(InstancedRenderableComponent<T, N>& renderable) {
    m_renderables.push_back(renderable);
    m_do_full_instance_data_update = true;
  }

  void remove_renderable(InstancedRenderableComponent<T, N>& renderable) {
    vector_remove(m_renderables, renderable);
    m_do_full_instance_data_update = true;
  }

  void update_instance_data(InstancedRenderableComponent<T, N>& renderable) {
    unsigned int idx = std::find(m_renderables.begin(), m_renderables.end(), renderable) - m_renderables.begin();
    auto data = renderable.get_instance_data();
    if (renderable.is_dirty()) renderable.set_clean();
    m_vertex_array->get_instance_buffer().bind();
    m_vertex_array->get_instance_buffer().copy_subregion(idx * sizeof(N), sizeof(N), &data);
    m_vertex_array->get_instance_buffer().unbind();
  }

  void update_instance_buffer() {
    std::vector<N> instance_data;
    instance_data.reserve(m_vertex_array->get_instance_buffer().get_count());
    for (auto& renderable : m_renderables) {
      instance_data.push_back(renderable.get().get_instance_data());
      if (renderable.get().is_dirty()) renderable.get().set_clean();
    }
    m_vertex_array->get_instance_buffer().bind();
    m_vertex_array->get_instance_buffer().copy(instance_data);
    m_vertex_array->get_instance_buffer().unbind();
    m_do_full_instance_data_update = false;
    m_renderables_to_update.clear();
  }

  void schedule_instance_data_update(InstancedRenderableComponent<T, N>& renderable) {
    m_renderables_to_update.push_back(renderable);
  }

 protected:
  InstancedRenderPipeline(ShaderProgram& shader_program, DrawPrimitiveType type,
                          std::unique_ptr<InstancedVertexArray<T, N>> vertex_array)
      : BaseRenderPipeline<T>(shader_program, type), m_vertex_array(std::move(vertex_array)) {}

  void flush_scheduled_instance_data_updates() {
    if (m_renderables_to_update.size() == m_renderables.size()) {
      update_instance_buffer();
    } else {
      m_vertex_array->get_instance_buffer().bind();
      for (auto& renderable : m_renderables_to_update) {
        unsigned int idx = std::find(m_renderables.begin(), m_renderables.end(), renderable) - m_renderables.begin();
        auto data = renderable.get().get_instance_data();
        m_vertex_array->get_instance_buffer().copy_subregion(idx * sizeof(N), sizeof(N), &data);
        renderable.get().set_clean();
      }
      m_vertex_array->get_instance_buffer().unbind();
    }
    m_renderables_to_update.clear();
  }

  std::vector<std::reference_wrapper<InstancedRenderableComponent<T, N>>> m_renderables;
  std::vector<std::reference_wrapper<InstancedRenderableComponent<T, N>>> m_renderables_to_update;
  std::unique_ptr<InstancedVertexArray<T, N>> m_vertex_array;
  bool m_do_full_instance_data_update = false;

  friend class RenderPipelineBuilder;
};
}  // namespace mge

#endif  // MGE_RENDERER_PIPELINE_RENDER_PIPELINEste