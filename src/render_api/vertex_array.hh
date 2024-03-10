#ifndef MGE_RENDER_API_VERTEX_ARRAY_HH
#define MGE_RENDER_API_VERTEX_ARRAY_HH

#include "../mgepch.hh"

#include "opengl_utils.hh"

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
    glCheckError();
    glBindVertexArray(m_vertex_array_id);
    glCheckError();

    glGenBuffers(1, &m_vertex_buffer_id);
    glCheckError();
    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer_id);
    glCheckError();
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(T), vertices.data(),
                 GL_STATIC_DRAW);
    glCheckError();

    if (!indices.empty()) {
      glGenBuffers(1, &m_element_buffer_id);
      glCheckError();
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_element_buffer_id);
      glCheckError();
      glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                   indices.size() * sizeof(unsigned int), indices.data(),
                   GL_STATIC_DRAW);
      glCheckError();
    }

    size_t stride = 0;
    for (int i = 0; i < vertex_attributes.size(); ++i) {
      glVertexAttribPointer(i, vertex_attributes[i].size,
                            vertex_attributes[i].type, GL_FALSE, sizeof(T),
                            reinterpret_cast<void*>(stride));
      glCheckError();
      glEnableVertexAttribArray(i);
      glCheckError();
      stride +=
          vertex_attributes[i].size * gl_sizeof_type(vertex_attributes[i].type);
    }
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
  }

  inline void bind() const { glBindVertexArray(m_vertex_array_id); }
  inline void unbind() const { glBindVertexArray(0); }

  inline void update_vertices(std::vector<T> vertices) {
    GLint size = 0;
    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer_id);
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);

    if (size < vertices.size() * sizeof(T)) {
      glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(T),
                   vertices.data(), GL_STATIC_DRAW);
    } else {
      glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(T),
                      vertices.data());
    }
  }
  inline void update_indices(std::vector<unsigned int> indices) {
    GLint size = 0;
    glBindBuffer(GL_ARRAY_BUFFER, m_element_buffer_id);
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);

    if (size < indices.size() * sizeof(unsigned int)) {
      glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                   indices.size() * sizeof(unsigned int), indices.data(),
                   GL_STATIC_DRAW);
    } else {
      glBufferSubData(GL_ARRAY_BUFFER, 0, indices.size() * sizeof(unsigned int),
                      indices.data());
    }
  }

 private:
  unsigned int m_vertex_array_id = 0;
  unsigned int m_vertex_buffer_id = 0;
  unsigned int m_element_buffer_id = 0;
};
}  // namespace mge

#endif  // MGE_RENDER_API_VERTEX_ARRAY_HH