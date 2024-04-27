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
  auto& entity_ref = *entity;
  m_entities_by_tag.emplace(tag, std::move(entity));
  entity_ref.add_component<TagComponent>(tag);
  return entity_ref;
}

Entity& Scene::create_entity(const std::string& tag,
                             const std::function<void(Entity&)> func) {
  if (m_entities_by_tag.contains(tag)) {
    throw std::runtime_error("Entity tag duplicated");
  }

  auto entity = std::unique_ptr<Entity>(new Entity(*m_registry));
  auto& entity_ref = *entity;
  m_entities_by_tag.emplace(tag, std::move(entity));
  func(entity_ref);
  entity_ref.add_component<TagComponent>(tag);
  return entity_ref;
}

void Scene::destroy_entity(const std::string& tag) {
  if (!m_entities_by_tag.contains(tag)) {
    MGE_WARN("Entity {} doesn\'t exist", tag);
    return;
  }

  auto node = m_entities_by_tag.extract(tag);
  for (auto& child : node.mapped().get()->get_owned_children()) {
    m_entities_by_tag.erase(child.get().get_component<mge::TagComponent>());
  }
}

bool Scene::rename_entity(const std::string& old_tag,
                          const std::string& new_tag) {
  if (m_entities_by_tag.contains(new_tag)) {
    return false;
  }
  auto node = m_entities_by_tag.extract(old_tag);
  node.key() = new_tag;
  node.mapped()->patch<mge::TagComponent>(
      [&new_tag](auto& tag) { tag.set_tag(new_tag); });
  m_entities_by_tag.insert(std::move(node));
  return true;
}

void Scene::clear() { m_registry->clear(); }

}  // namespace mge
