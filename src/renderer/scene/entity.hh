#ifndef MGE_RENDERER_SCENE_ENTITY_HH
#define MGE_RENDERER_SCENE_ENTITY_HH

#include "../../mgepch.hh"

#include "../../utils.hh"

namespace mge {
class Entity;

using EntityId = entt::entity;
using EntityVector = std::vector<std::reference_wrapper<mge::Entity>>;
using OptionalEntity = std::optional<std::reference_wrapper<Entity>>;

class Entity {
 public:
  ~Entity() {
    if (!m_registry.valid(m_id)) return;
    destroy_recursively();
  }

  PREVENT_COPY(Entity);

  void destroy() {
    m_registry.destroy(m_id);
    for (auto& parent : m_parents) {
      vector_remove(parent.get().m_children, *this);
    }
    for (auto& child : m_children) {
      vector_remove(child.get().m_parents, *this);
    }
  }

  void destroy_recursively() {
    m_registry.destroy(m_id);
    for (auto& parent : m_parents) {
      vector_remove(parent.get().m_children, *this);
    }
    for (auto& child : m_children) {
      child.get().destroy_recursively();
    }
  }

  template <class T, class... Args>
  inline decltype(auto) add_component(Args&&... args) {
    return m_registry.emplace<T>(m_id, std::forward<Args>(args)...);
  }

  template <class T, class... Args>
  inline decltype(auto) add_or_replace_component(Args&&... args) {
    return m_registry.emplace_or_replace<T>(m_id, std::forward<Args>(args)...);
  }

  template <class T>
  inline const T& get_component() const {
    return m_registry.get<T>(m_id);
  }

  template <class T, class F>
  inline void patch(F func) {
    m_registry.patch<T>(m_id, func);
  }

  template <typename T>
  inline std::optional<const T&> try_get_component() const {
    auto component = m_registry.try_get<T>(m_id);
    return component ? *component : std::nullopt;
  }

  template <typename T>
  inline void remove_component() {
    m_registry.remove<T>(m_id);
  }

  template <typename T>
  inline bool has_component() const {
    return m_registry.try_get<T>(m_id);
  }

  inline bool operator<(const Entity& other) const { return m_id < other.m_id; }

  inline bool operator==(const Entity& other) const { return m_id == other.m_id; }

  inline void add_child(Entity& child) {
    child.m_parents.emplace_back(*this);
    m_children.emplace_back(child);
  }

  inline void remove_child(Entity& child) {
    child.m_parents.erase(std::remove(child.m_parents.begin(), child.m_parents.end(), *this), child.m_parents.end());
    m_children.erase(std::remove(m_children.begin(), m_children.end(), child), m_children.end());
  }

  inline EntityVector& get_children() { return m_children; }
  inline EntityVector& get_parents() { return m_parents; }

  inline void propagate(const std::function<void(Entity&)>& func) {
    for (auto& child : m_children) {
      child.get().run_and_propagate(func);
    }
  }

  inline void run_and_propagate(const std::function<void(Entity&)>& func) {
    func(*this);
    propagate(func);
  }

  inline const EntityId get_id() const { return m_id; }

 private:
  Entity(entt::registry& registry) : m_registry(registry), m_id(m_registry.create()) {}

  entt::registry& m_registry;
  EntityId m_id;
  EntityVector m_children;
  EntityVector m_parents;

  friend class Scene;
};

inline bool operator<(const std::reference_wrapper<Entity>& lhs, const Entity& rhs) { return lhs.get() < rhs; }
}  // namespace mge

#endif  // MGE_RENDERER_SCENE_ENTITY_HH