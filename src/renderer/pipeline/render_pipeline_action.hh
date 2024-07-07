#ifndef MGE_RENDERER_PIPELINE_RENDER_PIPELINE_ACTION
#define MGE_RENDERER_PIPELINE_RENDER_PIPELINE_ACTION

#include "../../mgepch.hh"

#include "../../render_api/buffer.hh"
#include "../../render_api/shader_program.hh"
#include "../../render_api/vertex_array.hh"

namespace mge {
enum class DrawPrimitiveType { POINT, LINE, LINE_STRIP, TRIANGLE, TRIANGLE_STRIP, PATCH };

inline GLenum draw_primitive_type_to_gl(DrawPrimitiveType type) {
  switch (type) {
    case DrawPrimitiveType::POINT:
      return GL_POINT;
    case DrawPrimitiveType::LINE:
      return GL_LINE;
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

class RenderPipelineAction {
 public:
  RenderPipelineAction() = default;
  virtual ~RenderPipelineAction() = default;

  PREVENT_COPY(RenderPipelineAction);

  virtual void run() = 0;
  virtual void cleanup() = 0;

 protected:
};

class BindShaderProgramAction : public RenderPipelineAction {
 public:
  BindShaderProgramAction(ShaderProgram& shader_program) : RenderPipelineAction(), m_shader_program(shader_program) {}
  virtual ~BindShaderProgramAction() override = default;

  PREVENT_COPY(BindShaderProgramAction);

  virtual void run() override { m_shader_program.bind(); }
  virtual void cleanup() override { m_shader_program.try_unbind(); }

  const ShaderProgram& get_shader_program() const { return m_shader_program; }

 private:
  ShaderProgram& m_shader_program;
};

class BindVertexArrayAction : public RenderPipelineAction {
 public:
  BindVertexArrayAction(VertexArray& vertex_array) : RenderPipelineAction(), m_vertex_array(vertex_array) {}
  virtual ~BindVertexArrayAction() override = default;

  PREVENT_COPY(BindVertexArrayAction);

  virtual void run() override { m_vertex_array.bind(); }
  virtual void cleanup() override { m_vertex_array.try_unbind(); }

 private:
  VertexArray& m_vertex_array;
};

class SendDrawCallAction : public RenderPipelineAction {
 public:
  SendDrawCallAction(DrawPrimitiveType type = DrawPrimitiveType::TRIANGLE, unsigned int size = 0,
                     bool is_indexed = false, unsigned int instance_count = 0, unsigned int patch_count = 0)
      : RenderPipelineAction(),
        m_type(type),
        m_size(size),
        m_is_indexed(is_indexed),
        m_instance_count(instance_count),
        m_patch_count(patch_count) {}
  virtual ~SendDrawCallAction() override = default;

  PREVENT_COPY(SendDrawCallAction);

  virtual void run() override {
    if (m_patch_count > 0) {
      RenderContext::get_instance().set_patch_count(m_patch_count);
    }
    if (m_instance_count > 0) {
      if (m_is_indexed) {
        RenderContext::get_instance().draw_instanced_elements(draw_primitive_type_to_gl(m_type), m_size,
                                                              m_instance_count);
      } else {
        RenderContext::get_instance().draw_instanced(draw_primitive_type_to_gl(m_type), m_size, m_instance_count);
      }
    } else {
      if (m_is_indexed) {
        RenderContext::get_instance().draw_elements(draw_primitive_type_to_gl(m_type), m_size);
      } else {
        RenderContext::get_instance().draw(draw_primitive_type_to_gl(m_type), m_size);
      }
    }
  }
  virtual void cleanup() override {}

 private:
  unsigned int m_size;
  bool m_is_indexed;
  unsigned int m_instance_count;
  unsigned int m_patch_count;
  DrawPrimitiveType m_type;
};

template <class T>
class UpdateUniformAction : public RenderPipelineAction {
 public:
  UpdateUniformAction(const std::string& uniform_name, ShaderProgram& shader_program,
                      const std::function<T(void)>& update_func)
      : RenderPipelineAction(),
        m_uniform_name(uniform_name),
        m_shader_program(shader_program),
        m_update_func(update_func) {}
  virtual ~UpdateUniformAction() override = default;

  PREVENT_COPY(UpdateUniformAction);

  virtual void run() override { m_shader_program.set_uniform_value(m_uniform_name, m_update_func()); }
  virtual void cleanup() override {}

 private:
  const std::string& m_uniform_name;
  ShaderProgram& m_shader_program;
  const std::function<T(void)>& m_update_func;
};

template <class T>
class BindBufferAction : public RenderPipelineAction {
 public:
  BindBufferAction(Buffer<T>& buffer) : RenderPipelineAction(), m_buffer(buffer) {}
  virtual ~BindBufferAction() override = default;

  PREVENT_COPY(BindBufferAction);

  virtual void run() override { m_buffer.bind(); }
  virtual void cleanup() override { m_buffer.try_unbind(); }

 private:
  Buffer<T>& m_buffer;
};

template <class T>
class UpdateBufferAction : public RenderPipelineAction {
 public:
  UpdateBufferAction(Buffer<T>& buffer, const std::function<std::pair<T*, unsigned int>(void)> update_func)
      : RenderPipelineAction(), m_buffer(buffer), m_update_func(update_func) {}
  virtual ~UpdateBufferAction() override = default;

  PREVENT_COPY(UpdateBufferAction);

  virtual void run() override {
    auto [data, size] = m_update_func();
    m_buffer.copy(data, size);
  }
  virtual void cleanup() override {}

 private:
  Buffer<T>& m_buffer;
  const std::function<std::pair<T*, unsigned int>(void)> m_update_func;
};
}  // namespace mge

#endif  // MGE_RENDERER_PIPELINE_RENDER_PIPELINE_ACTION