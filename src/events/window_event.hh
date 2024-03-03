#ifndef MGE_EVENTS_WINDOW_EVENT_HH
#define MGE_EVENTS_WINDOW_EVENT_HH

#include "../window.hh"
#include "event.hh"

namespace mge {

class WindowEvent : public Event {
 public:
  WindowEvent(const Window& window) : m_window(window) {}
  virtual inline const std::string name() const override {
    return "WindowEvent";
  }
  inline const Window& get_window() const { return m_window; }

 protected:
  const Window& m_window;
};

class WindowClosedEvent : public WindowEvent {
 public:
  WindowClosedEvent(const Window& window) : WindowEvent(window) {}
  virtual inline const std::string name() const override {
    return "WindowClosedEvent";
  }
};

class WindowFramebufferResizedEvent : public WindowEvent {
 public:
  WindowFramebufferResizedEvent(const Window& window, uint16_t width,
                                uint16_t height)
      : WindowEvent(window), m_width(width), m_height(height) {}
  virtual inline const std::string name() const override {
    return "WindowFramebufferResizedEvent";
  }

  inline const uint16_t get_width() const { return m_width; }
  inline const uint16_t get_height() const { return m_height; }

 private:
  uint16_t m_width;
  uint16_t m_height;
};

}  // namespace mge

#endif  // MGE_EVENTS_WINDOW_EVENT_HH