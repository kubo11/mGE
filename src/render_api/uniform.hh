#ifndef MGE_RENDER_API_UNIFORM
#define MGE_RENDER_API_UNIFORM

#include "mgepch.hh"

namespace mge {
class Uniform {
 public:
  Uniform(const std::string& name, const GLint location, const GLenum type)
      : m_name(name), m_location(location), m_type(type), m_committed(true) {}

  virtual ~Uniform() = default;

  PREVENT_COPY(Uniform);

  inline const std::string& get_name() const { return m_name; }
  inline const GLint get_location() const { return m_location; }
  inline const GLenum get_type() const { return m_type; }

  inline void commit() {
    if (!m_committed) {
      commit_data();
      m_committed = true;
    }
  }

  template <typename T>
  inline void set_value(const T& value) {
    m_committed = m_committed && !set_data(value);
  }

 protected:
  virtual const char* get_type_name() const = 0;
  virtual void commit_data() = 0;

  bool set_data(bool value) { return set_data(static_cast<GLint>(value)); }
  bool set_data(glm::bvec2 value) { return set_data(glm::ivec2(value.x, value.y)); }
  bool set_data(glm::bvec3 value) { return set_data(glm::ivec3(value.x, value.y, value.z)); }
  bool set_data(glm::bvec4 value) { return set_data(glm::ivec4(value.x, value.y, value.z, value.w)); }

#define DECLARE_SET_DATA(type) \
  virtual bool set_data(const type& value) { return type_error(#type); }

  DECLARE_SET_DATA(GLfloat)
  DECLARE_SET_DATA(GLint)
  DECLARE_SET_DATA(GLuint)

  DECLARE_SET_DATA(glm::vec2)
  DECLARE_SET_DATA(glm::vec3)
  DECLARE_SET_DATA(glm::vec4)

  DECLARE_SET_DATA(glm::ivec2)
  DECLARE_SET_DATA(glm::ivec3)
  DECLARE_SET_DATA(glm::ivec4)

  DECLARE_SET_DATA(glm::uvec2)
  DECLARE_SET_DATA(glm::uvec3)
  DECLARE_SET_DATA(glm::uvec4)

  DECLARE_SET_DATA(glm::mat2x2)
  DECLARE_SET_DATA(glm::mat2x3)
  DECLARE_SET_DATA(glm::mat2x4)
  DECLARE_SET_DATA(glm::mat3x2)
  DECLARE_SET_DATA(glm::mat3x3)
  DECLARE_SET_DATA(glm::mat3x4)
  DECLARE_SET_DATA(glm::mat4x2)
  DECLARE_SET_DATA(glm::mat4x3)
  DECLARE_SET_DATA(glm::mat4x4)

#undef DECLARE_SET_DATA

 private:
  const std::string m_name;
  const GLint m_location;
  const GLenum m_type;
  bool m_committed;

  bool type_error(const char* name) {
    MGE_ASSERT(
        "Attempted to set shader uniform with wrong type ({}, instead of {}): "
        "{}",
        name, get_type_name(), get_name());
    return false;
  }
};
}  // namespace mge

#endif  // MGE_RENDER_API_UNIFORM