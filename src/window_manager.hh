#ifndef MGE_WINDOW_MANAGER_HH
#define MGE_WINDOW_MANAGER_HH

#include "mgepch.hh"

#include "window.hh"

namespace mge {
class WindowManager {
 public:
  ~WindowManager();

  static WindowManager& create();
  static WindowManager& get_instance();
  static Window& create_window(WindowData data);
  static void destroy_window(Window& window);
  static void glfw_error_callback(int error, const char* description);

  void terminate();

 private:
  static std::unique_ptr<WindowManager> s_instance;
  std::vector<std::unique_ptr<Window>> m_windows;

  WindowManager();

  void init();
};
}  // namespace mge

#endif  // MGE_WINDOW_MANAGER_HH