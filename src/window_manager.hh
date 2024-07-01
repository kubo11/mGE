#ifndef MGE_WINDOW_MANAGER_HH
#define MGE_WINDOW_MANAGER_HH

#include "mgepch.hh"

#include "window.hh"

namespace mge {
class WindowManager {
 public:
  ~WindowManager();

  static std::shared_ptr<WindowManager> create();
  static WindowManager& get_instance();
  static std::shared_ptr<Window> create_window(WindowData data);
  static void destroy_window(Window& window);
  static void glfw_error_callback(int error, const char* description);
  void set_swap_interval(int interval);

  void terminate();

 private:
  static std::shared_ptr<WindowManager> s_instance;
  std::vector<std::shared_ptr<Window>> m_windows;

  WindowManager();

  void init();
};
}  // namespace mge

#endif  // MGE_WINDOW_MANAGER_HH