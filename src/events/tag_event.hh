#ifndef MGE_EVENTS_TAG_EVENT
#define MGE_EVENTS_TAG_EVENT

#include "../mgepch.hh"

#include "../renderer/scene/entity.hh"
#include "event.hh"

namespace mge {
enum class TagEvents { Update };

class TagUpdateEvent : public mge::Event<TagEvents> {
 public:
  TagUpdateEvent(mge::EntityId id, const std::string& tag)
      : mge::Event<TagEvents>(TagEvents::Update, "TagUpdateEvent"), id(id), tag(tag) {}

  mge::EntityId id;
  std::string tag;
};

}  // namespace mge
#endif  // MGE_EVENTS_TAG_EVENT