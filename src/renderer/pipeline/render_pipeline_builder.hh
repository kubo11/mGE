#ifndef MGE_RENDERER_PIPELINE_RENDER_PIPELINE_BUILDER
#define MGE_RENDERER_PIPELINE_RENDER_PIPELINE_BUILDER

#include "../../mgepch.hh"

#include "../../render_api/buffer.hh"
#include "../../render_api/shader_program.hh"
#include "../../render_api/vertex_array.hh"
#include "render_pipeline.hh"

namespace mge {
class RenderPipelineBuilder {
 public:
  RenderPipelineBuilder();
  ~RenderPipelineBuilder() = default;

  PREVENT_COPY(RenderPipelineBuilder);

  RenderPipelineBuilder& add_shader_program(ShaderProgram& shader_program);
  RenderPipelineBuilder& add_vertex_array(VertexArray& vertex_array);
  RenderPipelineBuilder& add_draw_call(DrawPrimitiveType type = DrawPrimitiveType::TRIANGLE, unsigned int size = 0,
                                       bool indexed = false, unsigned int instances = 0, unsigned int patches = 0);
  template <class T>
  RenderPipelineBuilder& add_uniform_update(const std::string& name, const std::function<T(void)>& update_func) {
    for (auto& action : m_current_pipeline->m_actions | std::views::reverse) {
      try {
        auto& shader_action = dynamic_cast<BindShaderProgramAction&>(action);
        MGE_ASSERT(shader_action.get_shader_program().has_uniform(name),
                   "Uniform not present in shader program bound by render pipeline: {}", name);
        m_current_pipeline->m_actions.push_back(unique_cast<RenderPipelineAction>(
            std::move(std::make_unique<UpdateUniformAction>(name, shader_action.m_shader_program, update_func))));
        return *this;
      } catch (std::bad_cast) {
        continue;
      }
    }
    MGE_ASSERT(false, "Attempted to update uniform before binding any shader program in render pipeline: {}", name);
    return *this;
  }
  template <class T>
  RenderPipelineBuilder& add_buffer(Buffer<T>& buffer) {
    m_current_pipeline->m_actions.push_back(
        unique_cast<RenderPipelineAction>(std::move(std::make_unique<BindBufferAction<T>>(buffer))));
    if (buffer.get_type() == Buffer<T>::Type::ELEMENT_ARRAY) {
      m_is_element_buffer_bound = true;
    }
    return *this;
  }
  template <class T>
  RenderPipelineBuilder& add_buffer_update(Buffer<T>& buffer, const std::function<std::vector<T>(void)>& update_func) {
    add_buffer_update(buffer, [&update_func]() {
      auto data = update_func();
      return std::pair<T*, unsigned int>{data.data().data.size()};
    });
    return *this;
  }
  template <class T>
  RenderPipelineBuilder& add_buffer_update(Buffer<T>& buffer,
                                           const std::function<std::pair<T*, unsigned int>(void)>& update_func) {
    for (auto& action : m_current_pipeline->m_actions | std::views::reverse) {
      try {
        auto& buffer_action = dynamic_cast<BindBufferAction<T>&>(action);
        MGE_ASSERT(buffer_action.m_buffer == buffer, "Attempted to update buffer that is not bound by render pipeline");
        m_current_pipeline->m_actions.push_back(
            unique_cast<RenderPipelineAction>(std::move(std::make_unique<UpdateBufferAction>(buffer, update_func))));
        return *this;
      } catch (std::bad_cast) {
        continue;
      }
    }
    MGE_ASSERT(false, "Attempted to update buffer before binding any buffer in render pipeline");
    return *this;
  }
  template <class T>
  RenderPipelineBuilder& add_buffer_update(typename Buffer<T>::Type buffer_type,
                                           const std::function<std::vector<T>(void)>& update_func) {
    for (auto& action : m_current_pipeline->m_actions) {
      try {
        auto& buffer_action = dynamic_cast<BindBufferAction<T>&>(action);
        if (buffer_action.m_buffer.m_type == buffer_type) {
          add_buffer_update(buffer_action.m_buffer, update_func);
          return *this;
        }
      } catch (std::bad_cast) {
        continue;
      }
    }
    MGE_ASSERT(false, "Attempted to update buffer before binding buffer of that type in render pipeline");
    return *this;
  }
  template <class T>
  RenderPipelineBuilder& add_buffer_update(typename Buffer<T>::Type buffer_type,
                                           const std::function<std::pair<T*, unsigned int>(void)>& update_func) {
    for (auto& action : m_current_pipeline->m_actions) {
      try {
        auto& buffer_action = dynamic_cast<BindBufferAction<T>&>(action);
        if (buffer_action.m_buffer.m_type == buffer_type) {
          add_buffer_update(buffer_action.m_buffer, update_func);
          return *this;
        }
      } catch (std::bad_cast) {
        continue;
      }
    }
    MGE_ASSERT(false, "Attempted to update buffer before binding buffer of that type in render pipeline");
    return *this;
  }
  std::unique_ptr<RenderPipeline> build();
  void clear();

 private:
  std::unique_ptr<RenderPipeline> m_current_pipeline = nullptr;
  bool m_has_vertex_array = false;
  bool m_has_shader_program = false;
  bool m_is_element_buffer_bound = false;
  bool m_has_draw_call = false;
  bool m_is_patched = false;
  unsigned int m_next_draw_call_size = 0;
  unsigned int m_instance_count = 0;

  void generate_draw_call();
};
}  // namespace mge

#endif  // MGE_RENDERER_PIPELINE_RENDER_PIPELINE_BUILDER