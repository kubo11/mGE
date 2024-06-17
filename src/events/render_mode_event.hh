#ifndef MGE_EVENTS_RENDER_MODE_EVENT_HH
#define MGE_EVENTS_RENDER_MODE_EVENT_HH

#include "event.hh"

#include "../renderer/scene/components/renderable_component.hh"

namespace mge {
enum class RenderModeEvents { RenderModeUpdated };

class RenderModeUpdatedEvent : public Event<RenderModeEvents> {
 public:
  RenderModeUpdatedEvent()
      : Event<RenderModeEvents>(RenderModeEvents::RenderModeUpdated,
                                "RenderModeUpdatedEvent") {}
  virtual ~RenderModeUpdatedEvent() = default;
  std::string tag;
  RenderMode render_mode;
};
}  // namespace mge

#endif  // MGE_EVENTS_RENDER_MODE_EVENT_HH