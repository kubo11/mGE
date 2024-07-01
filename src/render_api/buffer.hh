#ifndef MGE_RENDER_API_BUFFER
#define MGE_RENDER_API_BUFFER

#include "../mgepch.hh"

#include "../render_context.hh"

namespace mge {
template <class T = float>
class Buffer {
 public:
  enum class Type { ARRAY = 0, ELEMENT_ARRAY = 1, UNIFORM = 2, TEXTURE = 3, SHADER_STORAGE = 4 };
  enum class AccessMode { READ = 0, WRITE = 1, READ_WRITE = 2 };
  enum class UsageHint { DRAW = 0, READ = 1, COPY = 2 };
  enum class FrequencyHint { STATIC = 0, DYNAMIC = 1, STREAM = 2 };

  inline Buffer(Type type = Type::ARRAY, UsageHint usage = UsageHint::DRAW,
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
  inline const unsigned int get_count() const { return m_size / sizeof(T); }

  inline void bind() { RenderContext::get_instance().bind_buffer(buffer_type_to_gl(m_type), m_id); };
  inline void unbind() { RenderContext::get_instance().unbind_buffer(buffer_type_to_gl(m_type), m_id); }
  inline bool is_bound() { return RenderContext::get_instance().get_bound_buffer(buffer_type_to_gl(m_type)) == m_id; }

  inline void* map(AccessMode mode) {
    return RenderContext::get_instance().map_buffer(buffer_type_to_gl(m_type), m_id, buffer_access_mode_to_gl(mode));
  }
  inline void unmap() { RenderContext::get_instance().unmap_buffer(buffer_type_to_gl(m_type), m_id); }
  inline bool is_mapped() { return RenderContext::get_instance().get_mapped_buffer(buffer_type_to_gl(m_type)) == m_id; }

  inline void copy(unsigned int size, const T* data) {
    RenderContext::get_instance().copy_buffer(buffer_type_to_gl(m_type), m_id, size, data, buffer_hint_to_gl(m_frequency, m_usage));
    m_size = size;
  }
  inline void copy(const std::vector<T>& data) { copy(data.size() * sizeof(T), data.data()); }
  inline void copy_subregion(unsigned int offset, unsigned int size, T* data) {
    RenderContext::get_instance().copy_buffer_subregion(buffer_type_to_gl(m_type), m_id, offset, size, data);
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

  inline static GLenum buffer_type_to_gl(Type type) {
    switch (type) {
    case Type::ARRAY:
      return GL_ARRAY_BUFFER;
    case Type::ELEMENT_ARRAY:
      return GL_ELEMENT_ARRAY_BUFFER;
    case Type::UNIFORM:
      return GL_UNIFORM_BUFFER;
    case Type::TEXTURE:
      return GL_TEXTURE_BUFFER;
    case Type::SHADER_STORAGE:
      return GL_SHADER_STORAGE_BUFFER;
    
    default:
      return GL_NONE;
    }
  }

  inline static GLenum buffer_access_mode_to_gl(AccessMode mode) {
    switch (mode) {
    case AccessMode::READ:
      return GL_READ_ONLY;
    case AccessMode::WRITE:
      return GL_WRITE_ONLY;
    case AccessMode::READ_WRITE:
      return GL_READ_WRITE;

    default:
      return GL_NONE;
    }
  }

  inline static GLenum buffer_hint_to_gl(FrequencyHint frequency, UsageHint usage) {
    switch (frequency) {
    case FrequencyHint::STATIC:
      switch (usage) {
      case UsageHint::DRAW:
        return GL_STATIC_DRAW;
      case UsageHint::READ:
        return GL_STATIC_READ;
      case UsageHint::COPY:
        return GL_STATIC_COPY;
      
      default:
        return GL_NONE;
      }
    case FrequencyHint::DYNAMIC:
      switch (usage) {
      case UsageHint::DRAW:
        return GL_DYNAMIC_DRAW;
      case UsageHint::READ:
        return GL_DYNAMIC_READ;
      case UsageHint::COPY:
        return GL_DYNAMIC_COPY;
      
      default:
        return GL_NONE;
      }
    case FrequencyHint::STREAM:
      switch (usage) {
      case UsageHint::DRAW:
        return GL_STREAM_DRAW;
      case UsageHint::READ:
        return GL_STREAM_READ;
      case UsageHint::COPY:
        return GL_STREAM_COPY;
      
      default:
        return GL_NONE;
      }
    
    default:
      return GL_NONE;
    }
  }
};
}  // namespace mge

#endif  // MGE_RENDER_API_BUFFER