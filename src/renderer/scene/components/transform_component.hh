#ifndef MGE_RENDERER_COMPONENTS_TRANSFORM_COMPONENT_HH
#define MGE_RENDERER_COMPONENTS_TRANSFORM_COMPONENT_HH

#include "../../../math/quat.hh"
#include "../../../mgepch.hh"

namespace mge {
struct TransformComponent {
  TransformComponent(const glm::vec3& position = {0.0f, 0.0f, 0.0f},
                     const quat& rotation = {1.0f, 0.0f, 0.0f, 0.0f},
                     const glm::vec3& scale = {1.0f, 1.0f, 1.0f})
      : m_position(position),
        m_rotation(rotation),
        m_scale(scale),
        m_model_mat(1.0f) {
    update_model_mat();
  }
  TransformComponent(TransformComponent&& other)
      : m_position(std::move(other.m_position)),
        m_rotation(std::move(other.m_rotation)),
        m_scale(std::move(other.m_scale)),
        m_model_mat(std::move(other.m_model_mat)) {}
  inline TransformComponent& operator=(TransformComponent&& other) {
    m_position = std::move(other.m_position);
    m_rotation = std::move(other.m_rotation);
    m_scale = std::move(other.m_scale);
    m_model_mat = std::move(other.m_model_mat);
    return *this;
  }

  inline void translate(const glm::vec3& offset) {
    m_position += offset;
    update_model_mat();
  }
  inline void rotate(float angle, const glm::vec3& axis) {
    m_rotation *= angleAxis(angle, glm::normalize(axis));
    m_rotation = normalize(m_rotation);
    update_model_mat();
  }
  inline void scale(float scale) {
    m_scale *= scale;
    update_model_mat();
  }

  inline void set_position(const glm::vec3& position) {
    m_position = position;
    update_model_mat();
  }
  inline void set_rotation(const quat& rotation) {
    m_rotation = rotation;
    update_model_mat();
  }
  inline void set_scale(const glm::vec3& scale) {
    m_scale = scale;
    update_model_mat();
  }

  inline const glm::vec3& get_position() const { return m_position; }
  inline const quat& get_rotation() const { return m_rotation; }
  inline const glm::vec3& get_scale() const { return m_scale; }

  inline const glm::mat4& get_model_mat() const { return m_model_mat; }

 private:
  glm::vec3 m_position;
  quat m_rotation;
  glm::vec3 m_scale;
  glm::mat4 m_model_mat;

  inline void update_model_mat() {
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), m_position);
    glm::mat4 rotation = mat4_cast(m_rotation);
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), m_scale);

    m_model_mat = translation * rotation * scale;
  }
};
}  // namespace mge

#endif  // MGE_RENDERER_COMPONENTS_TRANSFORM_COMPONENT_HH