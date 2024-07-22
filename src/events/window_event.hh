#ifndef MGE_EVENTS_WINDOW_EVENT_HH
#define MGE_EVENTS_WINDOW_EVENT_HH

#include "../window.hh"
#include "event.hh"

namespace mge {
enum class WindowEvents { WindowClosed, WindowFramebufferResized };

class WindowClosedEvent : public Event<WindowEvents> {
 public:
  WindowClosedEvent(const Window& window)
      : Event<WindowEvents>(WindowEvents::WindowClosed, "WindowClosedEvent"), window(window) {}
  virtual ~WindowClosedEvent() = default;
  const Window& window;
};

class WindowFramebufferResizedEvent : public Event<WindowEvents> {
 public:
  WindowFramebufferResizedEvent(const Window& window, uint16_t width, uint16_t height)
      : Event<WindowEvents>(WindowEvents::WindowFramebufferResized, "WindowFramebufferResizedEvent"),
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