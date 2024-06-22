#ifndef MGE_RENDER_API_BUFFER
#define MGE_RENDER_API_BUFFER

#include "mgepch.hh"

#include "render_context.hh"

namespace {
constexpr std::array<GLenum, 5> buffer_type_to_gl = {GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER, GL_UNIFORM_BUFFER,
                                                     GL_TEXTURE_BUFFER, GL_SHADER_STORAGE_BUFFER};

constexpr std::array<GLenum, 3> buffer_access_mode_to_gl = {GL_READ_ONLY, GL_WRITE_ONLY, GL_READ_WRITE};

constexpr std::array<std::array<GLenum, 3>, 3> buffer_hint_to_gl = {
    {{GL_STATIC_DRAW, GL_STATIC_READ, GL_STATIC_COPY},
     {GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, GL_DYNAMIC_COPY},
     {GL_STREAM_DRAW, GL_STREAM_READ, GL_STREAM_COPY}}};
};  // namespace

namespace mge {
template <class T = float>
class Buffer {
 public:
  enum class Type { ARRAY = 0, ELEMENT_ARRAY = 1, UNIFORM = 2, TEXTURE = 3, SHADER_STORAGE = 4 };
  enum class AccessMode { READ = 0, WRITE = 1, READ_WRITE = 2 };
  enum class UsageHint { DRAW = 0, READ = 1, COPY = 2 };
  enum class FrequencyHint { STATIC = 0, DYNAMIC = 1, STREAM = 2 };

  inline Buffer(Type type = Type::ARRAY, UsageHint usage = UsageHint::STATIC,
                FrequencyHint frequency = FrequencyHint::STATIC)
      : m_id(RenderContext::get_instance().create_buffer()),
        m_data{},
        m_size(0),
        m_type(type),
        m_usage(usage),
        m_frequency(frequency) {}
  inline virtual ~Buffer() { destroy(); }

  inline void destroy() {
    if (m_id) RenderContext::get_instance().destroy_buffer(m_id);
  }

  inline const unsigned int get_size() const { return m_size; }

  inline void bind() { RenderContext::get_instance().bind_buffer(buffer_type_to_gl(m_type), m_id); };
  inline void unbind() { RenderContext::get_instance().unbind_buffer(buffer_type_to_gl[m_type], m_id); }
  inline bool is_bound() { return RenderContext::get_instance().get_bound_buffer(buffer_type_to_gl[m_type]) == m_id; }

  inline void* map(AccessMode mode) {
    return RenderContext::get_instance().map_buffer(buffer_type_to_gl(m_type), m_id, buffer_access_mode_to_gl[mode]);
  }
  inline void unmap() { RenderContext::get_instance().unmap_buffer(buffer_type_to_gl(m_type), m_id); }
  inline bool is_mapped() { return RenderContext::get_instance().get_mapped_buffer(buffer_type_to_gl[m_type]) == m_id; }

  inline void copy(unsigned int size, T* data) {
    RenderContext::get_instance().copy_buffer(m_type, size, data, buffer_hint_to_gl[m_frequency][m_usage]);
  }
  inline void copy(const std::vector<T>& data) { copy(data.size() * sizeof(T), data.data()); }
  inline void copy_subregion(unsigned int offset, unsigned int size, T* data) {
    RenderContext::get_instance().copy_buffer_subregion(m_type, offset, size, data);
  }
  inline void copy_subregion(unsigned int offset, const std::vector<T>& data) {
    copy_subregion(offset, data.size() * sizeof(T), data.data());
  }

  inline void stage(std::vector<T>&& data) { m_data = data; }
  inline void flush() {
    copy(m_data);
    std::vector<T> empty;
    m_data.swap(empty);
  }
  inline bool is_staged() { return !m_data.empty(); }

 private:
  unsigned int m_size;
  std::vector<T> m_data;
  GLuint m_id;

  const Type m_type;
  const UsageHint m_usage;
  const FrequencyHint m_frequency;
};
}  // namespace mge

#endif  // MGE_RENDER_API_BUFFER