#ifndef MGE_RENDERER_SCENE_ENTITY_HH
#define MGE_RENDERER_SCENE_ENTITY_HH

#include "../../mgepch.hh"

namespace mge {
class Entity {
 public:
  ~Entity() { m_registry.destroy(m_entity); }

  PREVENT_COPY(Entity);

  template <class T, class... Args>
  inline T& add_component(Args&&... args) {
    return m_registry.emplace<T>(m_entity, std::forward<Args>(args)...);
  }

  template <class T>
  inline T& get_component() {
    return m_registry.get<T>(m_entity);
  }

  template <typename T>
  inline std::optional<T&> try_get_component() {
    auto component = m_registry.try_get<T>(m_entity);
    return component ? *component : std::nullopt;
  }

  template <typename T>
  void remove_component() {
    m_registry.remove<T>(m_entity);
  }

  template <typename T>
  bool has_component() {
    return m_registry.try_get<T>(m_entity);
  }

 private:
  Entity(entt::registry& registry)
      : m_registry(registry), m_entity(m_registry.create()) {}

  entt::registry& m_registry;
  entt::entity m_entity;

  friend class Scene;
};
}  // namespace mge

#endif  // MGE_RENDERER_SCENE_ENTITY_HH