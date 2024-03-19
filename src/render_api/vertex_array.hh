#ifndef MGE_RENDER_API_VERTEX_ARRAY_HH
#define MGE_RENDER_API_VERTEX_ARRAY_HH

#include "../mgepch.hh"
#include "../render_context.hh"

namespace mge {
struct VertexAttribute {
  unsigned int size;
  GLenum type;
};

template <class T>
class VertexArray {
 public:
  inline VertexArray(std::vector<T> vertices,
                     std::vector<VertexAttribute> vertex_attributes,
                     std::vector<unsigned int> indices = {}) {
    glGenVertexArrays(1, &m_vertex_array_id);
    glBindVertexArray(m_vertex_array_id);

    glGenBuffers(1, &m_vertex_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer_id);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(T), vertices.data(),
                 GL_STATIC_DRAW);

    m_count = vertices.size();

    if (!indices.empty()) {
      glGenBuffers(1, &m_element_buffer_id);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_element_buffer_id);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                   indices.size() * sizeof(unsigned int), indices.data(),
                   GL_STATIC_DRAW);
      m_count = indices.size();
    }

    size_t stride = 0;
    for (int i = 0; i < vertex_attributes.size(); ++i) {
      glVertexAttribPointer(i, vertex_attributes[i].size,
                            vertex_attributes[i].type, GL_FALSE, sizeof(T),
                            reinterpret_cast<void*>(stride));
      glEnableVertexAttribArray(i);
      stride += vertex_attributes[i].size *
                RenderContext::glSizeofType(vertex_attributes[i].type);
    }

    glCheckError();
  }

  inline ~VertexArray() {
    if (m_vertex_array_id) {
      glDeleteVertexArrays(1, &m_vertex_array_id);
    }
    if (m_vertex_buffer_id) {
      glDeleteBuffers(1, &m_vertex_buffer_id);
    }
    if (m_element_buffer_id) {
      glDeleteBuffers(1, &m_element_buffer_id);
    }

    glCheckError();
  }

  inline void bind() const {
    glBindVertexArray(m_vertex_array_id);
    glCheckError();
  }
  inline void unbind() const {
    glBindVertexArray(0);
    glCheckError();
  }

  inline void update_vertices(std::vector<T> vertices) {
    GLint size = 0;
    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer_id);
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);

    if (!m_element_buffer_id) {
      m_count = vertices.size();
    }

    if (size < vertices.size() * sizeof(T)) {
      glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(T),
                   vertices.data(), GL_STATIC_DRAW);
    } else {
      glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(T),
                      vertices.data());
    }

    glCheckError();
  }
  inline void update_indices(std::vector<unsigned int> indices) {
    GLint size = 0;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_element_buffer_id);
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);

    m_count = indices.size();

    if (size < indices.size() * sizeof(unsigned int)) {
      glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                   indices.size() * sizeof(unsigned int), indices.data(),
                   GL_STATIC_DRAW);
    } else {
      glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0,
                      indices.size() * sizeof(unsigned int), indices.data());
    }

    glCheckError();
  }

  inline unsigned int get_count() const { return m_count; }

 private:
  unsigned int m_vertex_array_id = 0;
  unsigned int m_vertex_buffer_id = 0;
  unsigned int m_element_buffer_id = 0;
  unsigned int m_count = 0;
};
}  // namespace mge

#endif  // MGE_RENDER_API_VERTEX_ARRAY_HH