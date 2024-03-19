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

class WindowMouseMovedEvent : public WindowEvent {
 public:
  WindowMouseMovedEvent(const Window& window, glm::vec2 beg, glm::vec2 end)
      : WindowEvent(window), m_beg(beg), m_end(end) {}
  virtual inline const std::string name() const override {
    return "WindowMouseMovedEvent";
  }

  inline glm::vec2 get_beg() const { return m_beg; }
  inline glm::vec2 get_end() const { return m_end; }
  inline glm::vec2 get_offset() const { return m_end - m_beg; }

 private:
  glm::vec2 m_beg;
  glm::vec2 m_end;
};

class WindowScrollEvent : public WindowEvent {
 public:
  WindowScrollEvent(const Window& window, float y_offset)
      : WindowEvent(window), m_y_offset(y_offset) {}
  virtual inline const std::string name() const override {
    return "WindowScrollEvent";
  }

  inline float get_y_offset() const { return m_y_offset; }

 private:
  float m_y_offset;
};

class WindowMousePressedEvent : public WindowEvent {
 public:
  WindowMousePressedEvent(const Window& window, int button, glm::vec2 position)
      : WindowEvent(window), m_button(button), m_position(position) {}
  virtual inline const std::string name() const override {
    return "WindowMousePressedEvent";
  }

  inline int get_button() const { return m_button; }
  inline glm::vec2 get_position() const { return m_position; }

 private:
  int m_button;
  glm::vec2 m_position;
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