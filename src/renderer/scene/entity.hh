#ifndef MGE_RENDERER_SCENE_ENTITY_HH
#define MGE_RENDERER_SCENE_ENTITY_HH

#include "../../mgepch.hh"

#include "../../utils.hh"

namespace mge {
class Entity;

using EntityId = entt::entity;
using EntityVector = std::vector<std::reference_wrapper<mge::Entity>>;
using OptionalEntity = std::optional<std::reference_wrapper<Entity>>;
using Handler = std::function<void(mge::Entity&)>;
using HandlerMap = std::unordered_map<TypeInfoRef, std::list<Handler>, Hasher, EqualTo>;

class Entity {
 public:
  ~Entity() {
    if (!m_registry.valid(m_id)) return;
    destroy_recursively();
  }

  PREVENT_COPY(Entity);

  void destroy() {
    if (is_valid()) m_registry.destroy(m_id);
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

  bool is_valid() const { return m_registry.valid(m_id); }

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
    vector_remove(child.m_parents, *this);
    vector_remove(m_children, child);
  }

  inline void remove_all_children() {
    for (auto& child : m_children) {
      vector_remove(child.get().m_parents, *this);
    }
    EntityVector empty_vector;
    m_children.swap(empty_vector);
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

  template <class T, class N>
  unsigned int register_on_construct(void (N::*func_ptr)(mge::Entity&), N* arg) {
    return register_on_construct<T>(std::bind(func_ptr, arg, std::placeholders::_1));
  }

  template <class T>
  unsigned int register_on_construct(const Handler& handler) {
    m_on_construct_handlers[typeid(T)].push_back(handler);
    int handle = m_next_handler_id++;
    m_handler_handles[handle] = std::prev(m_on_construct_handlers.at(typeid(T)).end());
    return handle;
  }

  template <class T>
  void unregister_on_construct(unsigned int handle) {
    auto it = m_handler_handles.find(handle);
    if (it != m_handler_handles.end()) {
      m_on_construct_handlers.at(typeid(T)).erase(it->second);
    }
  }

  template <class T, class N>
  unsigned int register_on_update(void (N::*func_ptr)(mge::Entity&), N* arg) {
    return register_on_update<T>(std::bind(func_ptr, arg, std::placeholders::_1));
  }

  template <class T>
  unsigned int register_on_update(const Handler& handler) {
    m_on_update_handlers[typeid(T)].push_back(handler);
    int handle = m_next_handler_id++;
    m_handler_handles[handle] = std::prev(m_on_update_handlers.at(typeid(T)).end());
    return handle;
  }

  template <class T>
  void unregister_on_update(unsigned int handle) {
    auto it = m_handler_handles.find(handle);
    if (it != m_handler_handles.end()) {
      m_on_update_handlers.at(typeid(T)).erase(it->second);
    }
  }

  template <class T, class N>
  unsigned int register_on_destroy(void (N::*func_ptr)(mge::Entity&), N* arg) {
    return register_on_destroy<T>(std::bind(func_ptr, arg, std::placeholders::_1));
  }

  template <class T>
  unsigned int register_on_destroy(const Handler& handler) {
    m_on_destroy_handlers[typeid(T)].push_back(handler);
    int handle = m_next_handler_id++;
    m_handler_handles[handle] = std::prev(m_on_destroy_handlers.at(typeid(T)).end());
    return handle;
  }

  template <class T>
  void unregister_on_destroy(unsigned int handle) {
    auto it = m_handler_handles.find(handle);
    if (it != m_handler_handles.end()) {
      m_on_destroy_handlers.at(typeid(T)).erase(it->second);
    }
  }

 private:
  Entity(entt::registry& registry) : m_registry(registry), m_id(m_registry.create()) {}

  template <class T>
  void on_construct() {
    for (auto& handler : m_on_construct_handlers[typeid(T)]) {
      handler(*this);
    }
  }

  template <class T>
  void on_update() {
    for (auto& handler : m_on_update_handlers[typeid(T)]) {
      handler(*this);
    }
  }

  template <class T>
  void on_destroy() {
    for (auto& handler : m_on_destroy_handlers[typeid(T)]) {
      handler(*this);
    }
  }

  entt::registry& m_registry;
  EntityId m_id;
  EntityVector m_children;
  EntityVector m_parents;
  HandlerMap m_on_construct_handlers;
  HandlerMap m_on_update_handlers;
  HandlerMap m_on_destroy_handlers;
  unsigned int m_next_handler_id = 0u;
  std::unordered_map<unsigned int, typename std::list<Handler>::iterator> m_handler_handles;

  friend class Scene;
};

inline bool operator<(const std::reference_wrapper<Entity>& lhs, const Entity& rhs) { return lhs.get() < rhs; }
}  // namespace mge

#endif  // MGE_RENDERER_SCENE_ENTITY_HH