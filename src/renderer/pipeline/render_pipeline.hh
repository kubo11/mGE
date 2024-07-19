#ifndef MGE_RENDERER_PIPELINE_RENDER_PIPELINE
#define MGE_RENDERER_PIPELINE_RENDER_PIPELINE

#include "../../mgepch.hh"

#include "../../render_api/shader_program.hh"
#include "../../render_api/vertex_array.hh"
#include "../scene/components/renderable_component.hh"
#include "render_pipeline_action.hh"

namespace mge {
template <class T>
class RenderPipeline {
 public:
  ~RenderPipeline() = default;

  PREVENT_COPY(RenderPipeline);

  void run() {
    m_shader_program.bind();
    for (auto& action : m_uniform_actions) {
      action->run();
    }
    for (auto& action : m_buffer_actions) {
      action->run();
    }

    if (m_patch_count > 0) {
      RenderContext::get_instance().set_patch_count(m_patch_count);
    }

    for (auto& renderable : m_renderables) {
      renderable.get().draw();
    }

    for (auto& action : m_buffer_actions | std::views::reverse) {
      action->cleanup();
    }
    for (auto& action : m_uniform_actions | std::views::reverse) {
      action->cleanup();
    }
    m_shader_program.unbind();
  }

  void add_renderable(RenderableComponent<T>& renderable) { m_renderables.push_back(renderable); }

  void remove_renderable(RenderableComponent<T>& renderable) {
    m_renderables.erase(std::remove(m_renderables.begin(), m_renderables.end(), renderable), m_renderables.end());
  }

  template <class N>
  void dynamic_uniform_update(const std::string& uniform_name, const std::function<N(void)>& update_func) {
    MGE_ASSERT(m_shader_program.has_uniform(uniform_name), "Attempted to update unknown uniform: {}", uniform_name);
    m_shader_program.set_uniform_value(uniform_name, update_func());
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

 private:
  RenderPipeline(ShaderProgram& shader_program) : m_shader_program{shader_program} {}

  ShaderProgram& m_shader_program;
  std::vector<std::reference_wrapper<RenderableComponent<T>>> m_renderables;
  std::vector<std::unique_ptr<RenderPipelineAction>> m_uniform_actions;
  std::vector<std::unique_ptr<RenderPipelineAction>> m_buffer_actions;
  unsigned int m_patch_count = 0;

  friend class RenderPipelineBuilder;
};
}  // namespace mge

#endif  // MGE_RENDERER_PIPELINE_RENDER_PIPELINEste