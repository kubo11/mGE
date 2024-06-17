#ifndef MGE_UI_MANAGER_HH
#define MGE_UI_MANAGER_HH

#include "mgepch.hh"

#include "window.hh"

namespace mge {
class UIManager {
 public:
  ~UIManager();

  static std::shared_ptr<UIManager> create(Window& window);
  static void start_frame();
  static void end_frame();
  void init(Window& window);
  void terminate();

 private:
  static std::shared_ptr<UIManager> s_instance;

  UIManager(Window& window);
};
}  // namespace mge

#endif  // MGE_UI_MANAGER_HH