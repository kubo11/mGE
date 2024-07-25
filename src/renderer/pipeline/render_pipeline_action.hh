#ifndef MGE_RENDERER_PIPELINE_RENDER_PIPELINE_ACTION
#define MGE_RENDERER_PIPELINE_RENDER_PIPELINE_ACTION

#include "../../mgepch.hh"

#include "../../render_api/buffer.hh"
#include "../../render_api/shader_program.hh"
#include "../../render_api/vertex_array.hh"

namespace mge {
class RenderPipelineAction {
 public:
  RenderPipelineAction() = default;
  virtual ~RenderPipelineAction() = default;

  PREVENT_COPY(RenderPipelineAction);

  virtual void run() = 0;
  virtual void cleanup() = 0;

 protected:
};

template <class T>
class UniformAction : public RenderPipelineAction {
 public:
  UniformAction(const std::string& uniform_name, ShaderProgram& shader_program,
                const std::function<T(void)>& update_func)
      : RenderPipelineAction(),
        m_uniform_name(uniform_name),
        m_shader_program(shader_program),
        m_update_func(update_func) {}
  virtual ~UniformAction() override = default;

  PREVENT_COPY(UniformAction);

  virtual void run() override { m_shader_program.set_uniform_value(m_uniform_name, m_update_func()); }
  virtual void cleanup() override {}

 private:
  std::string m_uniform_name;
  ShaderProgram& m_shader_program;
  std::function<T(void)> m_update_func;

  // template <class N>
  // friend class RenderPipeline<N>;
};

template <class T>
class BufferAction : public RenderPipelineAction {
 public:
  BufferAction(
      Buffer<T>& buffer, const std::function<std::pair<T*, unsigned int>(void)>& update_func =
                             []() {
                               return std::pair<T*, unsigned int>{nullptr, 0};
                             })
      : RenderPipelineAction(), m_buffer(buffer), m_update_func(update_func) {}
  virtual ~BufferAction() override = default;

  PREVENT_COPY(BufferAction);

  virtual void run() override {
    m_buffer.bind();
    auto [data, size] = m_update_func();
    if (data == nullptr || size == 0) return;
    m_buffer.copy(data, size);
  }
  virtual void cleanup() override { m_buffer.try_unbind(); }

 private:
  Buffer<T>& m_buffer;
  const std::function<std::pair<T*, unsigned int>(void)> m_update_func;

  // friend class RenderPipeline;
};
}  // namespace mge

#endif  // MGE_RENDERER_PIPELINE_RENDER_PIPELINE_ACTION