#ifndef MGE_EVENTS_WINDOW_EVENT_HH
#define MGE_EVENTS_WINDOW_EVENT_HH

#include "../window.hh"
#include "event.hh"

namespace mge {
enum class WindowEvents {
  WindowClosed,
  WindowMouseMoved,
  WindowScroll,
  WindowMousePressed,
  WindowFramebufferResized
};

class WindowClosedEvent : public Event<WindowEvents> {
 public:
  WindowClosedEvent(const Window& window)
      : Event<WindowEvents>(WindowEvents::WindowClosed, "WindowClosedEvent"),
        window(window) {}
  virtual ~WindowClosedEvent() = default;
  const Window& window;
};

class WindowMouseMovedEvent : public Event<WindowEvents> {
 public:
  WindowMouseMovedEvent(const Window& window, glm::vec2 beg, glm::vec2 end)
      : Event<WindowEvents>(WindowEvents::WindowMouseMoved,
                            "WindowMouseMovedEvent"),
        window(window),
        beg(beg),
        end(end) {}
  virtual ~WindowMouseMovedEvent() = default;
  const Window& window;
  glm::vec2 beg;
  glm::vec2 end;
};

class WindowScrollEvent : public Event<WindowEvents> {
 public:
  WindowScrollEvent(const Window& window, float y_offset)
      : Event<WindowEvents>(WindowEvents::WindowScroll, "WindowScrollEvent"),
        window(window),
        y_offset(y_offset) {}
  virtual ~WindowScrollEvent() = default;
  const Window& window;
  float y_offset;
};

class WindowMousePressedEvent : public Event<WindowEvents> {
 public:
  WindowMousePressedEvent(const Window& window, int button, glm::vec2 position)
      : Event<WindowEvents>(WindowEvents::WindowMousePressed,
                            "WindowMousePressedEvent"),
        window(window),
        button(button),
        position(position) {}
  virtual ~WindowMousePressedEvent() = default;
  const Window& window;
  int button;
  glm::vec2 position;
};

class WindowFramebufferResizedEvent : public Event<WindowEvents> {
 public:
  WindowFramebufferResizedEvent(const Window& window, uint16_t width,
                                uint16_t height)
      : Event<WindowEvents>(WindowEvents::WindowFramebufferResized,
                            "WindowFramebufferResizedEvent"),
        window(window),
        width(width),
        height(height) {}
  virtual ~WindowFramebufferResizedEvent() = default;
  const Window& window;
  uint16_t width;
  uint16_t height;
};
}  // namespace mge

#endif  // MGE_EVENTS_WINDOW_EVENT_HH