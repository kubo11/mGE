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

  inline Camera& get_current_camera() const { return m_current_camera; }
  inline void set_current_camera(unsigned int idx) {
    if (idx < m_cameras.size()) m_current_camera = *m_cameras[idx];
  }
  inline void add_camera(std::unique_ptr<Camera> camera) { m_cameras.push_back(std::move(camera)); }

  inline Entity& get_entity(const std::string& tag) { return *m_entities_by_tag.at(tag); }
  inline bool contains(const std::string& tag) { return m_entities_by_tag.contains(tag); }

  template <class T>
  inline void draw(std::function<void(std::shared_ptr<Shader> shader, mge::Entity&)> func = nullptr) const {
    auto draw_group = m_registry->group<>(entt::get<RenderableComponent<T>, TagComponent>);

    draw_group.each([this, &func](auto entity, auto& renderable, auto& tag) {
      if (!renderable.is_enabled()) {
        return;
      }
      auto shader = renderable.get_shader();
      auto& vertex_array = renderable.get_vertex_array();

      shader->use();
      func(shader, *m_entities_by_tag.at(tag));
      vertex_array.bind();

      if (renderable.get_render_mode() == RenderMode::SOLID) {
        if (vertex_array.has_indices()) {
        } else {
        }
      } else if (renderable.get_render_mode() == RenderMode::WIREFRAME) {
        if (vertex_array.has_indices()) {
        } else {
        }
      }

      vertex_array.unbind();
      shader->unuse();
    });
  }

  template <class GetT, class ExcludeT>
  inline void foreach (GetT get, ExcludeT exclude, std::function<void(mge::Entity&)> func) {
    auto group = m_registry->group<>(get, exclude);
    group.each([this, &func](entt::entity entity, auto&...) {
      func(*m_entities_by_tag.at(m_registry->get<mge::TagComponent>(static_cast<const entt::entity>(entity))));
    });
  }

  template <class GetT, class ExcludeT>
  inline void foreach (GetT get, ExcludeT exclude, std::function<void(const mge::Entity&)> func) const {
    const auto group = m_registry->group<>(get, exclude);
    group.each([this, &func](entt::entity entity, auto&...) {
      func(*m_entities_by_tag.at(m_registry->get<mge::TagComponent>(static_cast<const entt::entity>(entity))));
    });
  }

  template <class GetT, class ExcludeT>
  inline unsigned int size(GetT get, ExcludeT exclude = entt::exclude_t<>()) const {
    return m_registry->group<>(get, exclude).size();
  }

  Entity& create_entity(const std::string& tag);
  Entity& create_entity(const std::string& tag, const std::function<void(Entity&)> func);
  void destroy_entity(const std::string& tag);
  bool rename_entity(const std::string& old_tag, const std::string& new_tag);
  void clear();
  template <class T>
  inline void clear() {
    m_registry->clear<T>();
  }

  template <class T, auto listener>
  inline void invoke_on_self(entt::registry& registry, entt::entity entity) {
    auto& my_entity = *m_entities_by_tag.at(registry.get<TagComponent>(entity));
    auto& component = registry.get<T>(entity);
    std::invoke(listener, component, my_entity);
  }

  template <class T, auto listener, class I>
  inline void on_construct(I& instance) {
    m_registry->on_construct<T>().template connect<listener>(instance);
  }
  template <class T, auto listener>
  inline void on_construct() {
    m_registry->on_construct<T>().template connect<&Scene::invoke_on_self<T, listener>>(*this);
  }
  template <class T, auto listener, class I>
  inline void on_update(I& instance) {
    m_registry->on_update<T>().template connect<listener>(instance);
  }
  template <class T, auto listener>
  inline void on_update() {
    m_registry->on_update<T>().template connect<&Scene::invoke_on_self<T, listener>>(*this);
  }
  template <class T, auto listener, class I>
  inline void on_destroy(I& instance) {
    m_registry->on_destroy<T>().template connect<listener>(instance);
  }
  template <class T, auto listener>
  inline void on_destroy() {
    m_registry->on_destroy<T>().template connect<&Scene::invoke_on_self<T, listener>>(*this);
  }
  template <class T>
  inline void disconnect_all() {
    m_registry->on_construct<T>().disconnect();
    m_registry->on_update<T>().disconnect();
    m_registry->on_destroy<T>().disconnect();
  }

 private:
  std::unique_ptr<entt::registry> m_registry;
  std::vector<std::unique_ptr<Camera>> m_cameras;
  Camera& m_current_camera;
  std::unordered_map<std::string, std::unique_ptr<Entity>> m_entities_by_tag;
};
}  // namespace mge

#endif  // MGE_RENDERER_SCENE_HH