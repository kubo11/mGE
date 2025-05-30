#ifndef MGE_WINDOW_HH
#define MGE_WINDOW_HH

#include "mgepch.hh"

#include "events/event.hh"
#include "events/inputs.hh"

namespace mge {
struct WindowData {
  uint16_t width;
  uint16_t height;
  std::string title;

  WindowData(const std::string &title = "mGE application", uint16_t width = 1280, uint16_t height = 720)
      : title(title), width(width), height(height) {}
};

class Window {
 public:
  Window(WindowData data);
  ~Window();

  void init();
  void update();
  void clear();
  void make_context_current();

  inline const uint16_t get_width() const { return m_data.width; }
  inline const uint16_t get_height() const { return m_data.height; }
  inline const float get_aspect_ratio() const {
    return static_cast<float>(m_data.width) / static_cast<float>(m_data.height);
  }
  inline const std::string &get_title() const { return m_data.title; }
  inline GLFWwindow *get_instance() { return m_window; }

  bool operator==(Window &w);
  bool operator==(const Window &w);

  GLFWwindowposfun set_position_callback(GLFWwindowposfun callback);
  GLFWwindowsizefun set_size_callback(GLFWwindowsizefun callback);
  GLFWwindowclosefun set_close_callback(GLFWwindowclosefun callback);
  GLFWwindowrefreshfun set_refresh_callback(GLFWwindowrefreshfun callback);
  GLFWwindowfocusfun set_focus_callback(GLFWwindowfocusfun callback);
  GLFWwindowiconifyfun set_iconify_callback(GLFWwindowiconifyfun callback);
  GLFWwindowmaximizefun set_maximize_callback(GLFWwindowmaximizefun callback);
  GLFWframebuffersizefun set_framebuffer_resize_callback(GLFWframebuffersizefun callback);
  GLFWwindowcontentscalefun set_content_scale_callback(GLFWwindowcontentscalefun callback);
  GLFWcursorposfun set_cursor_pos_callback(GLFWcursorposfun callback);
  GLFWscrollfun set_scroll_callback(GLFWscrollfun callback);
  GLFWmousebuttonfun set_mouse_button_callback(GLFWmousebuttonfun callback);
  GLFWkeyfun set_keyboard_key_callback(GLFWkeyfun callback);

  bool is_key_pressed(KeyboardKey key) const;
  bool is_mouse_pressed(MouseButton button) const;

 private:
  WindowData m_data;
  GLFWwindow *m_window;
  glm::vec4 m_clear_color;

  void set_default_window_callbacks();

  static void position_callback(GLFWwindow *window, int xpos, int ypos);
  static void size_callback(GLFWwindow *window, int width, int height);
  static void close_callback(GLFWwindow *window);
  static void refresh_callback(GLFWwindow *window);
  static void focus_callback(GLFWwindow *window, int focused);
  static void iconify_callback(GLFWwindow *window, int iconified);
  static void maximize_callback(GLFWwindow *window, int maximized);
  static void framebuffer_resize_callback(GLFWwindow *window, int width, int height);
  static void content_scale_callback(GLFWwindow *window, float xscale, float yscale);
  static void cursor_pos_callback(GLFWwindow *window, double x, double y);
  static void scroll_callback(GLFWwindow *window, double, double yOffset);
  static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
  static void keyboard_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
};
}  // namespace mge

#endif  // MGE_WINDOW_HH