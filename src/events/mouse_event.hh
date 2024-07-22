#ifndef MGE_EVENTS_MOUSE_EVENT_HH
#define MGE_EVENTS_MOUSE_EVENT_HH

#include "../mgepch.hh"

#include "../window.hh"
#include "event.hh"
#include "inputs.hh"

namespace mge {
enum class MouseEvents { MouseMoved, MouseScroll, MouseButtonPressed };

class MouseMovedEvent : public Event<MouseEvents> {
 public:
  MouseMovedEvent(const Window& source_window, glm::vec2 start_position, glm::vec2 end_position)
      : Event<MouseEvents>(MouseEvents::MouseMoved, "MouseMovedEvent"),
        source_window(source_window),
        start_position(start_position),
        end_position(end_position) {}
  virtual ~MouseMovedEvent() = default;
  const Window& source_window;
  glm::vec2 start_position;
  glm::vec2 end_position;
};

class MouseScrollEvent : public Event<MouseEvents> {
 public:
  MouseScrollEvent(const Window& source_window, float y_offset)
      : Event<MouseEvents>(MouseEvents::MouseScroll, "MouseScrollEvent"),
        source_window(source_window),
        y_offset(y_offset) {}
  virtual ~MouseScrollEvent() = default;
  const Window& source_window;
  float y_offset;
};

class MouseButtonPressedEvent : public Event<MouseEvents> {
 public:
  MouseButtonPressedEvent(const Window& source_window, glm::vec2 position, MouseButton button,
                          InputModifierKeyFlags mods = 0)
      : Event<MouseEvents>(MouseEvents::MouseButtonPressed, "MouseButtonPressedEvent"),
        source_window(source_window),
        button(button),
        mods(mods),
        position(position) {}
  virtual ~MouseButtonPressedEvent() = default;
  const Window& source_window;
  MouseButton button;
  InputModifierKeyFlags mods;
  glm::vec2 position;
};
}  // namespace mge

#endif  // MGE_EVENTS_MOUSE_EVENT_HH