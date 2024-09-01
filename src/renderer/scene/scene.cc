#include "scene.hh"

#include "entity.hh"

namespace mge {
Scene::Scene(std::unique_ptr<BaseCamera> camera)
    : m_registry(std::make_unique<entt::registry>()),
      m_cameras([](std::unique_ptr<BaseCamera> cam) {
        std::vector<std::unique_ptr<BaseCamera>> v;
        v.emplace_back(std::move(cam));
        return v;
      }(std::move(camera))),
      m_current_camera_idx(0) {}

Entity& Scene::create_entity() {
  auto entity = std::unique_ptr<Entity>(new Entity(*m_registry));
  auto& entity_ref = *entity;
  m_entities_by_id.emplace(entity_ref.get_id(), std::move(entity));
  return entity_ref;
}

Entity& Scene::create_entity(const std::function<void(Entity&)> func) {
  auto entity = std::unique_ptr<Entity>(new Entity(*m_registry));
  auto& entity_ref = *entity;
  m_entities_by_id.emplace(entity_ref.get_id(), std::move(entity));
  func(entity_ref);
  return entity_ref;
}

void Scene::destroy_entity(EntityId id) {
  MGE_ASSERT(m_entities_by_id.contains(id), "Failed to destroy entity. Entity doesn't exist.");

  auto node = m_entities_by_id.extract(id);
  node.mapped()->destroy();
}

void Scene::destroy_entity(Entity& entity) { destroy_entity(entity.get_id()); }

void Scene::destroy_entity_recursively(EntityId id) {
  MGE_ASSERT(!m_entities_by_id.contains(id), "Failed to destroy entity. Entity doesn't exist.");

  auto node = m_entities_by_id.extract(id);
  node.mapped().get()->destroy_recursively();
}

void Scene::destroy_entity_recursively(Entity& entity) { destroy_entity_recursively(entity.get_id()); }

void Scene::clear() {
  m_registry->clear();
  m_entities_by_id.clear();
}

}  // namespace mge
