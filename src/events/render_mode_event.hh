#ifndef MGE_EVENTS_RENDER_MODE_EVENT_HH
#define MGE_EVENTS_RENDER_MODE_EVENT_HH

#include "event.hh"

#include "../renderer/scene/components/renderable_component.hh"

namespace mge {
class RenderModeUpdatedEvent : public Event {
 public:
  RenderModeUpdatedEvent(const std::string& tag, RenderMode render_mode)
      : m_tag(tag), m_render_mode(render_mode) {}
  virtual inline const std::string name() const override {
    return "RenderModeUpdatedEvent";
  }
  inline const std::string& get_tag() const { return m_tag; }
  inline RenderMode get_render_mode() const { return m_render_mode; }

 private:
  std::string m_tag;
  RenderMode m_render_mode;
};
}  // namespace mge

#endif  // MGE_EVENTS_RENDER_MODE_EVENT_HH