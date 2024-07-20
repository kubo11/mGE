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
  RenderPipelineBuilder() = default;
  ~RenderPipelineBuilder() = default;

  PREVENT_COPY(RenderPipelineBuilder);

  RenderPipelineBuilder& add_shader_program(std::shared_ptr<ShaderProgram> shader_program);
  template <class T>
  RenderPipelineBuilder& add_uniform_update(const std::string& name, const std::function<T(void)>& update_func) {
    MGE_ASSERT(m_shader_program != nullptr, "Cannot update uniform in render pipeline without a shader program");
    MGE_ASSERT(m_shader_program->has_uniform(name),
               "Uniform not present in shader program bound by render pipeline: {}", name);
    m_uniform_actions.push_back(unique_cast<RenderPipelineAction>(
        std::move(std::make_unique<UniformAction>(name, &m_shader_program, update_func))));
    return *this;
  }
  template <class T>
  RenderPipelineBuilder& add_buffer(
      Buffer<T>& buffer, const std::function<std::vector<T>(void)>& update_func = []() { return std::vector<T>{}; }) {
    add_buffer(buffer, [&update_func]() {
      auto data = update_func();
      return std::pair<T*, unsigned int>{data.data(), data.size()};
    });
    return *this;
  }
  template <class T>
  RenderPipelineBuilder& add_buffer(
      Buffer<T>& buffer, const std::function<std::pair<T*, unsigned int>(void)>& update_func = []() {
        return std::pair<T*, unsigned int>{nullptr, 0};
      }) {
    m_buffer_actions.push_back(
        unique_cast<RenderPipelineAction>(std::move(std::make_unique<BufferAction<T>>(buffer, update_func))));
    return *this;
  }
  RenderPipelineBuilder& set_patch_count(unsigned int patch_count);
  template <class T>
  std::unique_ptr<RenderPipeline<T>> build(DrawPrimitiveType type) {
    MGE_ASSERT(m_shader_program != nullptr, "Cannot build render pipeline without shader program");
    auto pipeline = std::unique_ptr<RenderPipeline<T>>(new RenderPipeline<T>(*m_shader_program, type));
    pipeline->m_uniform_actions = std::move(m_uniform_actions);
    pipeline->m_buffer_actions = std::move(m_buffer_actions);
    pipeline->m_patch_count = m_patch_count;
    clear();
    return pipeline;
  }
  template <class T, class N>
  std::unique_ptr<InstancedRenderPipeline<T, N>> build_instanced(std::unique_ptr<VertexArray<T>> vertex_array) {
    MGE_ASSERT(m_shader_program != nullptr, "Cannot build instanced render pipeline without shader program");
    auto pipeline = std::unique_ptr<InstancedRenderPipeline<T, N>>(new InstancedRenderPipeline<T, N>(
        *m_shader_program, vertex_array->get_draw_primitive_type(), std::move(vertex_array)));
    pipeline->m_uniform_actions = std::move(m_uniform_actions);
    pipeline->m_buffer_actions = std::move(m_buffer_actions);
    pipeline->m_patch_count = m_patch_count;
    clear();
    return pipeline;
  }
  void clear();

 private:
  std::shared_ptr<ShaderProgram> m_shader_program = nullptr;
  std::vector<std::unique_ptr<RenderPipelineAction>> m_uniform_actions;
  std::vector<std::unique_ptr<RenderPipelineAction>> m_buffer_actions;
  unsigned int m_patch_count = 0;
};
}  // namespace mge

#endif  // MGE_RENDERER_PIPELINE_RENDER_PIPELINE_BUILDER