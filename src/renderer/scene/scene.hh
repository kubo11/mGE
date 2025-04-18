#ifndef MGE_RENDERER_SCENE_HH
#define MGE_RENDERER_SCENE_HH

#include "../mgepch.hh"

#include "../camera/base_camera.hh"
#include "components/components.hh"
#include "entity.hh"

namespace mge {

class Entity;

class Scene {
 public:
  Scene(std::unique_ptr<BaseCamera> camera);
  ~Scene() = default;

  BaseCamera& get_current_camera() { return *m_cameras[m_current_camera_idx]; }
  void set_current_camera(unsigned int idx) {
    if (idx < m_cameras.size()) m_current_camera_idx = idx;
  }
  void add_camera(std::unique_ptr<BaseCamera> camera) { m_cameras.push_back(std::move(camera)); }
  BaseCamera& get_camera(unsigned int idx) { return *m_cameras[idx]; }

  Entity& get_entity(const EntityId id) { return *m_entities_by_id.at(id); }
  bool contains(const EntityId id) const { return m_entities_by_id.contains(id); }

  template <class GetT, class ExcludeT>
  void foreach (GetT get, ExcludeT exclude, std::function<void(mge::Entity&)> func) {
    auto group = m_registry->group<>(get, exclude);
    group.each([this, &func](EntityId id, auto&...) { func(*m_entities_by_id.at(id)); });
  }

  template <class GetT, class ExcludeT>
  void foreach (GetT get, ExcludeT exclude, std::function<void(const mge::Entity&)> func) const {
    const auto group = m_registry->group<>(get, exclude);
    group.each([this, &func](EntityId id, auto&...) { func(*m_entities_by_id.at(id)); });
  }

  template <class GetT, class ExcludeT>
  unsigned int size(GetT get, ExcludeT exclude = entt::exclude_t<>()) const {
    return m_registry->group<>(get, exclude).size();
  }

  template <class T>
  void invoke_on_construct(entt::registry&, entt::entity id) {
    auto& entity = m_entities_by_id.at(id);
    entity.get()->on_construct<T>();
  }

  template <class T>
  void enable_on_construct_listeners() {
    m_registry->on_construct<T>().template connect<&Scene::invoke_on_construct<T>>(*this);
  }

  template <class T>
  void invoke_on_update(entt::registry&, entt::entity id) {
    auto& entity = m_entities_by_id.at(id);
    entity.get()->on_update<T>();
  }

  template <class T>
  void enable_on_update_listeners() {
    m_registry->on_update<T>().template connect<&Scene::invoke_on_update<T>>(*this);
  }

  template <class T>
  void invoke_on_destroy(entt::registry&, entt::entity id) {
    auto& entity = m_entities_by_id.at(id);
    entity.get()->on_destroy<T>();
  }

  template <class T>
  void enable_on_destroy_listeners() {
    m_registry->on_destroy<T>().template connect<&Scene::invoke_on_destroy<T>>(*this);
  }

  template <class T>
  void enable_all_listeners() {
    enable_on_construct_listeners<T>();
    enable_on_update_listeners<T>();
    enable_on_destroy_listeners<T>();
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
  std::vector<std::unique_ptr<BaseCamera>> m_cameras;
  unsigned int m_current_camera_idx;
  std::unordered_map<EntityId, std::unique_ptr<Entity>> m_entities_by_id;
};
}  // namespace mge

#endif  // MGE_RENDERER_SCENE_HH