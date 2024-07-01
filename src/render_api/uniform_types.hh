#ifndef MGE_RENDER_API_UNIFORM_TYPES
#define MGE_RENDER_API_UNIFORM_TYPES

#include "../mgepch.hh"

#include "uniform.hh"

namespace mge {

#define DECLARE_UNIFORM_TYPE(class_name, data_type, get_function, set_function, uniform_type)          \
  class class_name : public Uniform {                                                                  \
   public:                                                                                             \
    class_name(const std::string& name, const GLint location, const GLenum type, const GLuint program) \
        : Uniform(name, location, type), m_data{} {                                                    \
      MGE_ASSERT(type == uniform_type, "Invalid uniform type: {}", type);                              \
      get_function;                                                                                    \
    }                                                                                                  \
                                                                                                       \
   protected:                                                                                          \
    virtual void commit_data() override { set_function; }                                              \
                                                                                                       \
    virtual bool set_data(const data_type& value) override {                                           \
      if (m_data != value) {                                                                           \
        m_data = value;                                                                                \
        return true;                                                                                   \
      }                                                                                                \
      return false;                                                                                    \
    }                                                                                                  \
                                                                                                       \
    virtual const char* get_type_name() const override { return #data_type; }                          \
                                                                                                       \
   private:                                                                                            \
    data_type m_data;                                                                                  \
  };

#define DECLARE_UNIFORM_TYPE_SCALAR(class_name, data_type, get_function_name, set_function_name, uniform_type) \
  DECLARE_UNIFORM_TYPE(class_name, data_type, get_function_name(program, location, &m_data),                   \
                       set_function_name(get_location(), m_data), uniform_type)

DECLARE_UNIFORM_TYPE_SCALAR(FloatUniform, GLfloat, glGetUniformfv, glUniform1f, GL_FLOAT)
DECLARE_UNIFORM_TYPE_SCALAR(IntUniform, GLint, glGetUniformiv, glUniform1i, GL_INT)
DECLARE_UNIFORM_TYPE_SCALAR(UintUniform, GLuint, glGetUniformuiv, glUniform1ui, GL_UNSIGNED_INT)
DECLARE_UNIFORM_TYPE_SCALAR(BoolUniform, GLint, glGetUniformiv, glUniform1i, GL_BOOL)

#undef DECLARE_UNIFORM_TYPE_SCALAR

#define DECLARE_UNIFORM_TYPE_VECTOR(class_name, data_type, get_function_name, set_function_name, uniform_type) \
  DECLARE_UNIFORM_TYPE(class_name, data_type, get_function_name(program, location, glm::value_ptr(m_data)),    \
                       set_function_name(get_location(), 1, glm::value_ptr(m_data)), uniform_type)

DECLARE_UNIFORM_TYPE_VECTOR(Float2Uniform, glm::vec2, glGetUniformfv, glUniform2fv, GL_FLOAT_VEC2)
DECLARE_UNIFORM_TYPE_VECTOR(Float3Uniform, glm::vec3, glGetUniformfv, glUniform3fv, GL_FLOAT_VEC3)
DECLARE_UNIFORM_TYPE_VECTOR(Float4Uniform, glm::vec4, glGetUniformfv, glUniform4fv, GL_FLOAT_VEC4)

DECLARE_UNIFORM_TYPE_VECTOR(Int2Uniform, glm::ivec2, glGetUniformiv, glUniform2iv, GL_INT_VEC2)
DECLARE_UNIFORM_TYPE_VECTOR(Int3Uniform, glm::ivec3, glGetUniformiv, glUniform3iv, GL_INT_VEC3)
DECLARE_UNIFORM_TYPE_VECTOR(Int4Uniform, glm::ivec4, glGetUniformiv, glUniform4iv, GL_INT_VEC4)

DECLARE_UNIFORM_TYPE_VECTOR(Uint2Uniform, glm::uvec2, glGetUniformuiv, glUniform2uiv, GL_UNSIGNED_INT_VEC2)
DECLARE_UNIFORM_TYPE_VECTOR(Uint3Uniform, glm::uvec3, glGetUniformuiv, glUniform3uiv, GL_UNSIGNED_INT_VEC3)
DECLARE_UNIFORM_TYPE_VECTOR(Uint4Uniform, glm::uvec4, glGetUniformuiv, glUniform4uiv, GL_UNSIGNED_INT_VEC4)

DECLARE_UNIFORM_TYPE_VECTOR(Bool2Uniform, glm::ivec2, glGetUniformiv, glUniform2iv, GL_BOOL_VEC2)
DECLARE_UNIFORM_TYPE_VECTOR(Bool3Uniform, glm::ivec3, glGetUniformiv, glUniform3iv, GL_BOOL_VEC3)
DECLARE_UNIFORM_TYPE_VECTOR(Bool4Uniform, glm::ivec4, glGetUniformiv, glUniform4iv, GL_BOOL_VEC4)

#undef DECLARE_UNIFORM_TYPE_VECTOR

#define DECLARE_UNIFORM_TYPE_MATRIX(class_name, data_type, get_function_name, set_function_name, uniform_type) \
  DECLARE_UNIFORM_TYPE(class_name, data_type, get_function_name(program, location, glm::value_ptr(m_data)),    \
                       set_function_name(get_location(), 1, GL_FALSE, glm::value_ptr(m_data)), uniform_type)

DECLARE_UNIFORM_TYPE_MATRIX(Float2x2Uniform, glm::mat2x2, glGetUniformfv, glUniformMatrix2fv, GL_FLOAT_MAT2)
DECLARE_UNIFORM_TYPE_MATRIX(Float2x3Uniform, glm::mat2x3, glGetUniformfv, glUniformMatrix2x3fv, GL_FLOAT_MAT2x3)
DECLARE_UNIFORM_TYPE_MATRIX(Float2x4Uniform, glm::mat2x4, glGetUniformfv, glUniformMatrix2x4fv, GL_FLOAT_MAT2x4)
DECLARE_UNIFORM_TYPE_MATRIX(Float3x2Uniform, glm::mat3x2, glGetUniformfv, glUniformMatrix3x2fv, GL_FLOAT_MAT3x2)
DECLARE_UNIFORM_TYPE_MATRIX(Float3x3Uniform, glm::mat3x3, glGetUniformfv, glUniformMatrix3fv, GL_FLOAT_MAT3)
DECLARE_UNIFORM_TYPE_MATRIX(Float3x4Uniform, glm::mat3x4, glGetUniformfv, glUniformMatrix3x4fv, GL_FLOAT_MAT3x4)
DECLARE_UNIFORM_TYPE_MATRIX(Float4x2Uniform, glm::mat4x2, glGetUniformfv, glUniformMatrix4x2fv, GL_FLOAT_MAT4x2)
DECLARE_UNIFORM_TYPE_MATRIX(Float4x3Uniform, glm::mat4x3, glGetUniformfv, glUniformMatrix4x3fv, GL_FLOAT_MAT4x3)
DECLARE_UNIFORM_TYPE_MATRIX(Float4x4Uniform, glm::mat4x4, glGetUniformfv, glUniformMatrix4fv, GL_FLOAT_MAT4)

#undef DECLARE_UNIFORM_TYPE_MATRIX

#undef DECLARE_UNIFORM_TYPE

inline std::unique_ptr<Uniform> create_uniform(const std::string& name, const GLint location, const GLenum type,
                                               const GLuint program) {
  switch (type) {
    case GL_FLOAT:
      return std::make_unique<FloatUniform>(name, location, type, program);
    case GL_INT:
      return std::make_unique<IntUniform>(name, location, type, program);
    case GL_UNSIGNED_INT:
      return std::make_unique<UintUniform>(name, location, type, program);
    case GL_BOOL:
      return std::make_unique<BoolUniform>(name, location, type, program);

    case GL_FLOAT_VEC2:
      return std::make_unique<Float2Uniform>(name, location, type, program);
    case GL_FLOAT_VEC3:
      return std::make_unique<Float3Uniform>(name, location, type, program);
    case GL_FLOAT_VEC4:
      return std::make_unique<Float4Uniform>(name, location, type, program);

    case GL_INT_VEC2:
      return std::make_unique<Int2Uniform>(name, location, type, program);
    case GL_INT_VEC3:
      return std::make_unique<Int3Uniform>(name, location, type, program);
    case GL_INT_VEC4:
      return std::make_unique<Int4Uniform>(name, location, type, program);

    case GL_UNSIGNED_INT_VEC2:
      return std::make_unique<Uint2Uniform>(name, location, type, program);
    case GL_UNSIGNED_INT_VEC3:
      return std::make_unique<Uint3Uniform>(name, location, type, program);
    case GL_UNSIGNED_INT_VEC4:
      return std::make_unique<Uint4Uniform>(name, location, type, program);

    case GL_BOOL_VEC2:
      return std::make_unique<Bool2Uniform>(name, location, type, program);
    case GL_BOOL_VEC3:
      return std::make_unique<Bool3Uniform>(name, location, type, program);
    case GL_BOOL_VEC4:
      return std::make_unique<Bool4Uniform>(name, location, type, program);

    case GL_FLOAT_MAT2:
      return std::make_unique<Float2x2Uniform>(name, location, type, program);
    case GL_FLOAT_MAT2x3:
      return std::make_unique<Float2x3Uniform>(name, location, type, program);
    case GL_FLOAT_MAT2x4:
      return std::make_unique<Float2x4Uniform>(name, location, type, program);
    case GL_FLOAT_MAT3x2:
      return std::make_unique<Float3x2Uniform>(name, location, type, program);
    case GL_FLOAT_MAT3:
      return std::make_unique<Float3x3Uniform>(name, location, type, program);
    case GL_FLOAT_MAT3x4:
      return std::make_unique<Float3x4Uniform>(name, location, type, program);
    case GL_FLOAT_MAT4x2:
      return std::make_unique<Float4x2Uniform>(name, location, type, program);
    case GL_FLOAT_MAT4x3:
      return std::make_unique<Float4x3Uniform>(name, location, type, program);
    case GL_FLOAT_MAT4:
      return std::make_unique<Float4x4Uniform>(name, location, type, program);

    default:
      MGE_ERROR("Invalid uniform type: {}", type);
      return nullptr;
  }
}

}  // namespace mge

#endif  // MGE_RENDER_API_UNIFORM_TYPES