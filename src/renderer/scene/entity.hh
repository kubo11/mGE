#ifndef MGE_RENDERER_SCENE_ENTITY_HH
#define MGE_RENDERER_SCENE_ENTITY_HH

#include "../../mgepch.hh"

namespace mge {
class Entity {
 public:
  ~Entity() { m_registry.destroy(m_entity); }

  PREVENT_COPY(Entity);

  template <class T, class... Args>
  inline decltype(auto) add_component(Args&&... args) {
    return m_registry.emplace<T>(m_entity, std::forward<Args>(args)...);
  }

  template <class T, class... Args>
  inline decltype(auto) add_or_replace_component(Args&&... args) {
    return m_registry.emplace_or_replace<T>(m_entity,
                                            std::forward<Args>(args)...);
  }

  template <class T>
  inline const T& get_component() const {
    return m_registry.get<T>(m_entity);
  }

  template <class T, class F>
  inline void patch(F func) {
    m_registry.patch<T>(m_entity, func);
  }

  template <typename T>
  inline std::optional<const T&> try_get_component() const {
    auto component = m_registry.try_get<T>(m_entity);
    return component ? *component : std::nullopt;
  }

  template <typename T>
  inline void remove_component() {
    m_registry.remove<T>(m_entity);
  }

  template <typename T>
  inline bool has_component() const {
    return m_registry.try_get<T>(m_entity);
  }

  inline bool operator<(const Entity& other) const {
    return m_entity < other.m_entity;
  }

  inline bool operator==(const Entity& other) const {
    return m_entity == other.m_entity;
  }

  inline void add_child(Entity& child) {
    child.m_parents.emplace_back(*this);
    m_children.emplace_back(child);
  }

  inline void remove_child(Entity& child) {
    child.m_parents.erase(
        std::remove(child.m_parents.begin(), child.m_parents.end(), *this),
        child.m_parents.end());
    m_children.erase(std::remove(m_children.begin(), m_children.end(), child),
                     m_children.end());
  }

  inline std::vector<std::reference_wrapper<Entity>>& get_children() {
    return m_children;
  }
  inline std::vector<std::reference_wrapper<Entity>>& get_parents() {
    return m_parents;
  }

  inline void propagate(const std::function<void(Entity&)>& func) {
    for (auto& child : m_children) {
      child.get().run_and_propagate(func);
    }
  }

  inline void run_and_propagate(const std::function<void(Entity&)>& func) {
    func(*this);
    propagate(func);
  }

 private:
  Entity(entt::registry& registry)
      : m_registry(registry), m_entity(m_registry.create()) {}

  entt::registry& m_registry;
  entt::entity m_entity;
  std::vector<std::reference_wrapper<Entity>> m_children;
  std::vector<std::reference_wrapper<Entity>> m_parents;

  friend class Scene;
};

inline bool operator<(const std::reference_wrapper<Entity>& lhs,
                      const Entity& rhs) {
  return lhs.get() < rhs;
}

using EntityVector = std::vector<std::reference_wrapper<mge::Entity>>;
using OptionalEntity = std::optional<std::reference_wrapper<Entity>>;

}  // namespace mge

#endif  // MGE_RENDERER_SCENE_ENTITY_HH