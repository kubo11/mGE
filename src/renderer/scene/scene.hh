#ifndef MGE_RENDERER_SCENE_HH
#define MGE_RENDERER_SCENE_HH

#include "../mgepch.hh"

#include "../camera.hh"
#include "components/components.hh"
#include "entity.hh"

namespace mge {

class Entity;

class Scene {
 public:
  Scene(std::unique_ptr<Camera> camera);
  ~Scene() = default;

  inline Camera& get_current_camera() { return m_current_camera; }
  inline void set_current_camera(unsigned int idx) {
    if (idx < m_cameras.size()) m_current_camera = *m_cameras[idx];
  }
  inline void add_camera(std::unique_ptr<Camera> camera) { m_cameras.push_back(std::move(camera)); }

  inline Entity& get_entity(EntityId id) { return *m_entities_by_id.at(id); }
  inline bool contains(EntityId id) { return m_entities_by_id.contains(id); }

  template <class GetT, class ExcludeT>
  inline void foreach (GetT get, ExcludeT exclude, std::function<void(mge::Entity&)> func) {
    auto group = m_registry->group<>(get, exclude);
    group.each([this, &func](EntityId id, auto&...) { func(*m_entities_by_id.at(id)); });
  }

  template <class GetT, class ExcludeT>
  inline void foreach (GetT get, ExcludeT exclude, std::function<void(const mge::Entity&)> func) const {
    const auto group = m_registry->group<>(get, exclude);
    group.each([this, &func](EntityId id, auto&...) { func(*m_entities_by_id.at(id)); });
  }

  template <class GetT, class ExcludeT>
  inline unsigned int size(GetT get, ExcludeT exclude = entt::exclude_t<>()) const {
    return m_registry->group<>(get, exclude).size();
  }

  Entity& create_entity();
  Entity& create_entity(const std::function<void(Entity&)> func);
  void destroy_entity(EntityId id);
  void destroy_entity(Entity& entity);
  void destroy_entity_recursively(EntityId id);
  void destroy_entity_recursively(Entity& entity);
  void clear();

 private:
  std::unique_ptr<entt::registry> m_registry;
  std::vector<std::unique_ptr<Camera>> m_cameras;
  Camera& m_current_camera;
  std::unordered_map<EntityId, std::unique_ptr<Entity>> m_entities_by_id;
};
}  // namespace mge

#endif  // MGE_RENDERER_SCENE_HH