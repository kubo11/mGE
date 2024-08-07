#include "window.hh"

#include "events/events.hh"

namespace mge {
Window::Window(WindowData data)
    : m_data(std::move(data)), m_clear_color(100.f / 256.0f, 100.f / 256.0f, 100.f / 256.0f, 1.0f) {
  MGE_INFO("Window \"{}\" created", m_data.title);
}

Window::~Window() {
  glfwDestroyWindow(m_window);
  m_window = nullptr;

  MGE_INFO("Window \"{}\" terminated", m_data.title);
}

void Window::init() {
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif  // __APPLE__

  m_window = glfwCreateWindow(m_data.width, m_data.height, m_data.title.c_str(), nullptr, nullptr);
  if (m_window == NULL) {
    throw std::runtime_error("GLFW cannot create a window instance");
  }

  glfwSetWindowUserPointer(m_window, this);

  set_default_window_callbacks();

  MGE_INFO("Window \"{}\" initialized", m_data.title);
}

void Window::update() {
  glfwSwapBuffers(m_window);
  glfwPollEvents();

  //   MGE_TRACE("Window \"{}\" updated", m_data.title);
}

void Window::clear() {
  glClearColor(m_clear_color.r, m_clear_color.g, m_clear_color.b, m_clear_color.a);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool Window::is_key_pressed(KeyboardKey key) const {
  return glfwGetKey(m_window, keyboard_key_to_glfw(key)) == GLFW_PRESS;
}

bool Window::is_mouse_pressed(MouseButton button) const {
  return glfwGetMouseButton(m_window, mouse_button_to_glfw(button)) == GLFW_PRESS;
}

void Window::make_context_current() { glfwMakeContextCurrent(m_window); }

bool Window::operator==(Window &w) { return m_window == w.m_window; }

bool Window::operator==(const Window &w) { return m_window == w.m_window; }

void Window::set_default_window_callbacks() {
  glfwSetWindowPosCallback(m_window, Window::position_callback);
  glfwSetWindowSizeCallback(m_window, Window::size_callback);
  glfwSetWindowCloseCallback(m_window, Window::close_callback);
  glfwSetWindowRefreshCallback(m_window, Window::refresh_callback);
  glfwSetWindowFocusCallback(m_window, Window::focus_callback);
  glfwSetWindowIconifyCallback(m_window, Window::iconify_callback);
  glfwSetWindowMaximizeCallback(m_window, Window::maximize_callback);
  glfwSetFramebufferSizeCallback(m_window, Window::framebuffer_resize_callback);
  glfwSetWindowContentScaleCallback(m_window, Window::content_scale_callback);
  glfwSetCursorPosCallback(m_window, Window::cursor_pos_callback);
  glfwSetScrollCallback(m_window, Window::scroll_callback);
  glfwSetMouseButtonCallback(m_window, Window::mouse_button_callback);
  glfwSetKeyCallback(m_window, Window::keyboard_key_callback);
}

void Window::position_callback(GLFWwindow *window, int xpos, int ypos) {}

void Window::size_callback(GLFWwindow *window, int width, int height) {}

void Window::close_callback(GLFWwindow *window) {
  Window *mge_window = static_cast<Window *>(glfwGetWindowUserPointer(window));
  WindowClosedEvent event(*mge_window);
  SendEvent(event);
}

void Window::refresh_callback(GLFWwindow *window) {}

void Window::focus_callback(GLFWwindow *window, int focused) {}

void Window::iconify_callback(GLFWwindow *window, int iconified) {}

void Window::maximize_callback(GLFWwindow *window, int maximized) {}

void Window::framebuffer_resize_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
  Window *mge_window = static_cast<Window *>(glfwGetWindowUserPointer(window));
  mge_window->m_data.width = width;
  mge_window->m_data.height = height;
  WindowFramebufferResizedEvent event(*mge_window, width, height);
  SendEvent(event);
}

void Window::cursor_pos_callback(GLFWwindow *window, double x, double y) {
  static float previous_x = 0.0f;
  static float previous_y = 0.0f;

  Window *mge_window = static_cast<Window *>(glfwGetWindowUserPointer(window));

  float curr_x = -1.0 + x / static_cast<double>(mge_window->m_data.width) * 2.0;
  float curr_y = 1.0 - y / static_cast<double>(mge_window->m_data.height) * 2.0;

  if (!ImGui::GetIO().WantCaptureMouse) {
    MouseMovedEvent event(*mge_window, {previous_x, previous_y}, {curr_x, curr_y});
    SendEvent(event);
  }

  previous_x = curr_x;
  previous_y = curr_y;
}

void Window::scroll_callback(GLFWwindow *window, double, double y_offset) {
  Window *mge_window = static_cast<Window *>(glfwGetWindowUserPointer(window));
  if (!ImGui::GetIO().WantCaptureMouse) {
    MouseScrollEvent event(*mge_window, y_offset);
    SendEvent(event);
  }
}

void Window::content_scale_callback(GLFWwindow *window, float xscale, float yscale) {}

void Window::mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
  Window *mge_window = static_cast<Window *>(glfwGetWindowUserPointer(window));
  if ((action == GLFW_PRESS || action == GLFW_RELEASE) && !ImGui::GetIO().WantCaptureMouse) {
    double pos_x, pos_y;
    glfwGetCursorPos(window, &pos_x, &pos_y);
    pos_x = -1.0 + pos_x / static_cast<double>(mge_window->m_data.width) * 2.0;
    pos_y = 1.0 - pos_y / static_cast<double>(mge_window->m_data.height) * 2.0;
    MouseButtonUpdatedEvent event(*mge_window, {pos_x, pos_y}, mouse_button_from_glfw(button),
                                  input_state_from_glfw(action), modifier_flags_from_glfw(mods));
    SendEvent(event);
  }
}

void Window::keyboard_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
  Window *mge_window = static_cast<Window *>(glfwGetWindowUserPointer(window));
  if (action == GLFW_PRESS || action == GLFW_RELEASE && !ImGui::GetIO().WantCaptureKeyboard) {
    KeyboardKeyUpdatedEvent event(*mge_window, keyboard_key_from_glfw(key), input_state_from_glfw(action),
                                  modifier_flags_from_glfw(mods));
    SendEvent(event);
  }
}

GLFWwindowposfun Window::set_position_callback(GLFWwindowposfun callback) {
  return glfwSetWindowPosCallback(m_window, callback);
}

GLFWwindowsizefun Window::set_size_callback(GLFWwindowsizefun callback) {
  return glfwSetWindowSizeCallback(m_window, callback);
}

GLFWwindowclosefun Window::set_close_callback(GLFWwindowclosefun callback) {
  return glfwSetWindowCloseCallback(m_window, callback);
}

GLFWwindowrefreshfun Window::set_refresh_callback(GLFWwindowrefreshfun callback) {
  return glfwSetWindowRefreshCallback(m_window, callback);
}

GLFWwindowfocusfun Window::set_focus_callback(GLFWwindowfocusfun callback) {
  return glfwSetWindowFocusCallback(m_window, callback);
}

GLFWwindowiconifyfun Window::set_iconify_callback(GLFWwindowiconifyfun callback) {
  return glfwSetWindowIconifyCallback(m_window, callback);
}

GLFWwindowmaximizefun Window::set_maximize_callback(GLFWwindowmaximizefun callback) {
  return glfwSetWindowMaximizeCallback(m_window, callback);
}

GLFWframebuffersizefun Window::set_framebuffer_resize_callback(GLFWframebuffersizefun callback) {
  return glfwSetFramebufferSizeCallback(m_window, callback);
}

GLFWwindowcontentscalefun Window::set_content_scale_callback(GLFWwindowcontentscalefun callback) {
  return glfwSetWindowContentScaleCallback(m_window, callback);
}

GLFWcursorposfun Window::set_cursor_pos_callback(GLFWcursorposfun callback) {
  return glfwSetCursorPosCallback(m_window, callback);
}

GLFWscrollfun Window::set_scroll_callback(GLFWscrollfun callback) { return glfwSetScrollCallback(m_window, callback); }

GLFWmousebuttonfun Window::set_mouse_button_callback(GLFWmousebuttonfun callback) {
  return glfwSetMouseButtonCallback(m_window, callback);
}

GLFWkeyfun Window::set_keyboard_key_callback(GLFWkeyfun callback) { return glfwSetKeyCallback(m_window, callback); }
}  // namespace mge
