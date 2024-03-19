#ifndef MGE_RENDERER_SCENE_HH
#define MGE_RENDERER_SCENE_HH

#include "../mgepch.hh"

#include "../camera.hh"
#include "../renderer.hh"
#include "components/components.hh"
#include "entity.hh"

namespace mge {

class Entity;

class Scene {
 public:
  Scene(std::unique_ptr<Camera> camera);
  ~Scene() {}

  inline Camera& get_current_camera() const { return m_current_camera; }
  inline void set_current_camera(unsigned int idx) {
    if (idx < m_cameras.size()) m_current_camera = *m_cameras[idx];
  }
  inline void add_camera(std::unique_ptr<Camera> camera) {
    m_cameras.push_back(std::move(camera));
  }

  inline Entity& get_entity(const std::string& tag) {
    return *m_entities_by_tag.at(tag);
  }
  inline bool contains(const std::string& tag) {
    return m_entities_by_tag.contains(tag);
  }
  std::optional<std::reference_wrapper<Entity>> get_closest_entity_ws(
      glm::vec3 position);
  std::optional<std::reference_wrapper<Entity>> get_closest_entity_ss(
      glm::vec2 position);

  template <class T>
  inline void draw(
      std::function<void(Shader&, mge::Entity&)> func = nullptr) const {
    auto draw_group = m_registry->group<>(
        entt::get<TransformComponent, RenderableComponent<T>, TagComponent>);

    draw_group.each([this, &func](auto entity, auto& transform,
                                  auto& renderable, auto& tag) {
      auto& shader = renderable.get_shader();
      auto& vertex_array = renderable.get_vertex_array();

      shader.use();
      func(shader, *m_entities_by_tag.at(tag));
      vertex_array.bind();

      if (renderable.get_render_mode() == RenderMode::SURFACE) {
        Renderer<RendererType::TRIANGLES>::draw_indexed(
            vertex_array.get_count());
      } else if (renderable.get_render_mode() == RenderMode::WIREFRAME) {
        Renderer<RendererType::LINES>::draw_indexed(vertex_array.get_count());
      }

      vertex_array.unbind();
      shader.unuse();
    });
  }

  Entity& create_entity(const std::string& tag);
  void destroy_entity(const std::string& tag);

 private:
  std::unique_ptr<entt::registry> m_registry;
  std::vector<std::unique_ptr<Camera>> m_cameras;
  Camera& m_current_camera;
  std::unordered_map<std::string, std::unique_ptr<Entity>> m_entities_by_tag;
};
}  // namespace mge

#endif  // MGE_RENDERER_SCENE_HH