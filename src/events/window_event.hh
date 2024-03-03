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

class WindowResizedEvent : public WindowEvent {};

}  // namespace mge

#endif  // MGE_EVENTS_WINDOW_EVENT_HH