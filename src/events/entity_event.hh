#ifndef MGE_EVENTS_ENTITY_EVENT
#define MGE_EVENTS_ENTITY_EVENT

#include "../mgepch.hh"

#include "../renderer/scene/entity.hh"
#include "event.hh"

namespace mge {
enum class EntityEvents { Added, Updated, Deleted, Delete, QueryById, QueryByTag, QueryByPosition };

class AddedEntityEvent : public Event<EntityEvents> {
 public:
  AddedEntityEvent(EntityId id) : Event<EntityEvents>(EntityEvents::Added, "AddedEntityEvent"), id(id) {}

  EntityId id;
};

class UpdatedEntityEvent : public Event<EntityEvents> {
 public:
  UpdatedEntityEvent(EntityId id) : Event<EntityEvents>(EntityEvents::Updated, "UpdatedEntityEvent"), id(id) {}

  EntityId id;
};

class DeletedEntityEvent : public Event<EntityEvents> {
 public:
  DeletedEntityEvent(EntityId id) : Event<EntityEvents>(EntityEvents::Deleted, "DeletedEntityEvent"), id(id) {}

  EntityId id;
};

class DeleteEntityEvent : public Event<EntityEvents> {
 public:
  DeleteEntityEvent(EntityId id) : Event<EntityEvents>(EntityEvents::Delete, "DeleteEntityEvent"), id(id) {}

  EntityId id;
};

class QueryEntityByIdEvent : public Event<EntityEvents> {
 public:
  QueryEntityByIdEvent(EntityId id)
      : Event<EntityEvents>(EntityEvents::QueryById, "QueryEntityByIdEvent"), id(id), entity(std::nullopt) {}

  EntityId id;
  OptionalEntity entity;
};

class QueryEntityByTagEvent : public Event<EntityEvents> {
 public:
  QueryEntityByTagEvent(const std::string& tag)
      : Event<EntityEvents>(EntityEvents::QueryByTag, "QueryEntityByTagEvent"), tag(tag), entity(std::nullopt) {}

  const std::string tag;
  OptionalEntity entity;
};

class QueryEntityByPositionEvent : public Event<EntityEvents> {
 public:
  QueryEntityByPositionEvent(glm::vec2 screen_space_position)
      : Event<EntityEvents>(EntityEvents::QueryByPosition, "QueryEntityByPositionEvent"),
        screen_space_position(screen_space_position),
        entity(std::nullopt) {}

  glm::vec2 screen_space_position;
  OptionalEntity entity;
};
}  // namespace mge

#endif  // MGE_EVENTS_ENTITY_EVENT