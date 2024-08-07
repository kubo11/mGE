#ifndef MGE_EVENTS_RENDER_MODE_EVENT
#define MGE_EVENTS_RENDER_MODE_EVENT

#include "event.hh"

#include "../renderer/scene/components/renderable_component.hh"

namespace mge {
enum class RenderModeEvents { RenderModeUpdated };

class RenderModeUpdatedEvent : public Event<RenderModeEvents> {
 public:
  RenderModeUpdatedEvent(EntityId id, RenderMode render_mode)
      : Event<RenderModeEvents>(RenderModeEvents::RenderModeUpdated, "RenderModeUpdatedEvent"),
        id(id),
        render_mode(render_mode) {}

  EntityId id;
  RenderMode render_mode;
};
}  // namespace mge

#endif  // MGE_EVENTS_RENDER_MODE_EVENT