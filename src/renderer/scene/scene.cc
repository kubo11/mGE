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

bool Scene::rename_entity(const std::string& old_tag,
                          const std::string& new_tag) {
  if (m_entities_by_tag.contains(new_tag)) {
    return false;
  }
  auto node = m_entities_by_tag.extract(old_tag);
  node.key() = new_tag;
  m_entities_by_tag.insert(std::move(node));
  return true;
}

}  // namespace mge
