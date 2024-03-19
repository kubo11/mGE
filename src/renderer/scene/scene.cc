#include "scene.hh"

#include "../renderer.hh"
#include "entity.hh"

namespace mge {
Scene::Scene(std::unique_ptr<Camera> camera)
    : m_registry(std::make_unique<entt::registry>()),
      m_cameras([](std::unique_ptr<Camera> cam) {
        std::vector<std::unique_ptr<Camera>> v;
        v.emplace_back(std::move(cam));
        return v;
      }(std::move(camera))),
      m_current_camera(*m_cameras[0]) {}

Entity& Scene::create_entity(const std::string& tag) {
  if (m_entities_by_tag.contains(tag)) {
    throw std::runtime_error("Entity tag duplicated");
  }

  auto entity = std::unique_ptr<Entity>(new Entity(*m_registry));
  entity->add_component<TagComponent>(tag);
  auto& entity_ref = *entity;
  m_entities_by_tag.emplace(tag, std::move(entity));
  return entity_ref;
}

void Scene::destroy_entity(const std::string& tag) {
  if (!m_entities_by_tag.contains(tag)) {
    MGE_WARN("Entity {} doesn\'t exist", tag);
    return;
  }

  m_entities_by_tag.erase(tag);
}

std::optional<std::reference_wrapper<Entity>> Scene::get_closest_entity_ws(
    glm::vec3 position) {
  auto draw_group =
      m_registry->group<>(entt::get<TransformComponent, TagComponent>);
  float min_dist = 10e7;
  std::string closest_tag = "";

  draw_group.each([this, &position, &min_dist, &closest_tag](
                      auto entity, auto& transform, auto& tag) {
    auto dist = glm::distance(position, transform.get_position());
    if (tag.get_tag() == "cursor") return;
    if (dist < min_dist && dist < 1.0f) {
      min_dist = dist;
      closest_tag = tag.get_tag();
    }
  });

  if (closest_tag.size()) {
    return *m_entities_by_tag.at(closest_tag);
  }

  return std::nullopt;
}

std::optional<std::reference_wrapper<Entity>> Scene::get_closest_entity_ss(
    glm::vec2 position) {
  auto draw_group =
      m_registry->group<>(entt::get<TransformComponent, TagComponent>);
  float min_dist = 10e7;
  std::string closest_tag = "";

  draw_group.each([this, &position, &min_dist, &closest_tag](
                      auto entity, auto& transform, auto& tag) {
    auto tmp = glm::vec2(m_current_camera.get_projection_view_matrix() *
                         glm::vec4(transform.get_position(), 1.0f));
    auto dist = glm::distance(
        position, glm::vec2(m_current_camera.get_projection_view_matrix() *
                            glm::vec4(transform.get_position(), 1.0f)));
    if (tag.get_tag() == "cursor") return;
    if (dist < min_dist && dist < 0.03f) {
      min_dist = dist;
      closest_tag = tag.get_tag();
    }
  });

  if (closest_tag.size()) {
    return *m_entities_by_tag.at(closest_tag);
  }

  return std::nullopt;
}

}  // namespace mge
