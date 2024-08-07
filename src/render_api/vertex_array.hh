#ifndef MGE_RENDER_API_VERTEX_ARRAY_HH
#define MGE_RENDER_API_VERTEX_ARRAY_HH

#include "../mgepch.hh"
#include "../render_context.hh"
#include "buffer.hh"

namespace mge {
struct VertexAttribute {
  GLuint size;
  GLenum type;
};

struct VertexInstanceAttribute {
  GLuint size;
  GLenum type;
  GLuint divisor;
};

template <class T>
class VertexArray {
 public:
  VertexArray(std::unique_ptr<Buffer<T>> vertex_buffer, const std::vector<VertexAttribute>& vertex_attributes,
              std::unique_ptr<ElementBuffer> element_buffer = nullptr)
      : m_id(RenderContext::get_instance().create_vertex_array()),
        m_last_attribute(0),
        m_vertex_buffer(std::move(vertex_buffer)),
        m_element_buffer(std::move(element_buffer)) {
    bind();
    attach_buffer(*m_vertex_buffer, vertex_attributes);
    unbind();
  }

  virtual ~VertexArray() { destroy(); }

  void destroy() {
    if (m_id) {
      RenderContext::get_instance().destroy_vertex_array(m_id);
    }
  }

  void bind() {
    RenderContext::get_instance().bind_vertex_array(m_id);
    if (has_element_buffer()) {
      m_element_buffer->bind();
    }
  }

  void unbind() {
    RenderContext::get_instance().unbind_vertex_array(m_id);
    if (has_element_buffer()) {
      m_element_buffer->unbind();
    }
  }

  void try_unbind() {
    RenderContext::get_instance().try_unbind_vertex_array(m_id);
    if (has_element_buffer()) {
      m_element_buffer->try_unbind();
    }
  }

  bool is_bound() const { return RenderContext::get_instance().get_bound_vertex_array() == m_id; }

  Buffer<T>& get_vertex_buffer() { return *m_vertex_buffer; }

  bool has_element_buffer() const { return m_element_buffer != nullptr; }

  ElementBuffer& get_element_buffer() {
    MGE_ASSERT(has_element_buffer(), "Attempted to return non existant element buffer");
    return *m_element_buffer;
  }

  unsigned int get_draw_size() const {
    return has_element_buffer() ? m_element_buffer->get_count() : m_vertex_buffer->get_count();
  }

 protected:
  GLuint m_id;
  GLuint m_last_attribute;
  std::unique_ptr<Buffer<T>> m_vertex_buffer;
  std::unique_ptr<ElementBuffer> m_element_buffer;

  void attach_buffer(Buffer<T>& buffer, const std::vector<VertexAttribute>& attributes) {
    buffer.bind();
    GLuint stride = 0;
    for (GLuint i = 0; i < attributes.size(); ++i) {
      RenderContext::get_instance().add_vertex_array_attribute(m_id, m_last_attribute + i, attributes[i].size,
                                                               attributes[i].type, sizeof(T),
                                                               reinterpret_cast<void*>(stride));
      stride += attributes[i].size * RenderContext::glSizeofType(attributes[i].type);
    }
    buffer.unbind();
    m_last_attribute += attributes.size();
  }
};

template <class T, class N>
class InstancedVertexArray : public VertexArray<T> {
 public:
  InstancedVertexArray(std::unique_ptr<Buffer<T>> vertex_buffer, const std::vector<VertexAttribute>& vertex_attributes,
                       std::unique_ptr<Buffer<N>> instance_buffer,
                       const std::vector<VertexInstanceAttribute>& instance_attributes,
                       std::unique_ptr<ElementBuffer> element_buffer = nullptr)
      : VertexArray<T>(std::move(vertex_buffer), vertex_attributes, std::move(element_buffer)),
        m_instance_buffer(std::move(instance_buffer)) {
    this->bind();
    attach_instance_buffer(*m_instance_buffer, instance_attributes);
    this->unbind();
  }

  Buffer<N>& get_instance_buffer() { return *m_instance_buffer; }

 protected:
  std::unique_ptr<Buffer<N>> m_instance_buffer;

  void attach_instance_buffer(Buffer<N>& buffer, const std::vector<VertexInstanceAttribute>& attributes) {
    buffer.bind();
    GLuint stride = 0;
    for (GLuint i = 0; i < attributes.size(); ++i) {
      RenderContext::get_instance().add_vertex_array_instanced_attribute(
          this->m_id, this->m_last_attribute + i, attributes[i].size, attributes[i].type, sizeof(N),
          reinterpret_cast<void*>(stride), attributes[i].divisor);
      stride += attributes[i].size * RenderContext::glSizeofType(attributes[i].type);
    }
    buffer.unbind();
    this->m_last_attribute += attributes.size();
  }
};
}  // namespace mge

#endif  // MGE_RENDER_API_VERTEX_ARRAY_HH