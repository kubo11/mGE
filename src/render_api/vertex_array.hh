#ifndef MGE_RENDER_API_VERTEX_ARRAY_HH
#define MGE_RENDER_API_VERTEX_ARRAY_HH

#include "../mgepch.hh"
#include "../render_context.hh"
#include "render_api/buffer.hh"

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

class VertexArray {
 public:
  inline VertexArray()
      : m_id(RenderContext::get_instance().create_vertex_array()), m_last_attribute(0), m_is_instanced(false) {}

  inline ~VertexArray() { destroy(); }

  inline void destroy() {
    if (m_id) {
      RenderContext::get_instance().destroy_vertex_array(m_id);
    }
  }

  inline void bind() { RenderContext::get_instance().bind_vertex_array(m_id); }

  inline void unbind() { RenderContext::get_instance().unbind_vertex_array(m_id); }

  inline bool is_bound() const { return RenderContext::get_instance().get_bound_vertex_array() == m_id; }

  template <class T>
  inline void attach_buffer(Buffer<T>& buffer, const std::vector<VertexAttribute>& attributes) {
    buffer.bind();
    GLuint max_attribute = m_last_attribute + attributes.size();
    GLuint stride = 0;
    for (GLuint i = 0; i < attributes.size(); ++i) {
      RenderContext::get_instance().add_vertex_array_attribute(m_id, m_last_attribute + i, attributes[i].size,
                                                               attributes[i].type, sizeof(T),
                                                               reinterpret_cast<void*>(stride));
    }
    buffer.unbind();
    m_last_attribute += attributes.size();
  }

  template <class T>
  inline void attach_instance_buffer(Buffer<T>& buffer, const std::vector<VertexInstanceAttribute>& attributes) {
    buffer.bind();
    GLuint stride = 0;
    for (GLuint i = 0; i < attributes.size(); ++i) {
      RenderContext::get_instance().add_vertex_array_instanced_attribute(
          m_id, m_last_attribute + i, attributes[i].size, attributes[i].type, sizeof(T),
          reinterpret_cast<void*>(stride), attributes[i].divisor);
    }
    buffer.unbind();
    m_last_attribute += attributes.size();
    m_is_instanced = true;
  }

  inline bool is_instanced() const { return m_is_instanced; }

 private:
  GLuint m_id;
  GLuint m_last_attribute;
  bool m_is_instanced;
};
}  // namespace mge

#endif  // MGE_RENDER_API_VERTEX_ARRAY_HH