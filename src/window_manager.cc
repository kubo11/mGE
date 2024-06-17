#include "window_manager.hh"

namespace mge {
std::shared_ptr<WindowManager> WindowManager::s_instance = nullptr;

std::shared_ptr<WindowManager> WindowManager::create() {
  s_instance = std::shared_ptr<WindowManager>(new WindowManager());
  MGE_INFO("Window manager created");
  s_instance->init();

  return s_instance;
}

WindowManager& WindowManager::get_instance() { return *WindowManager::s_instance; }

Window& WindowManager::create_window(WindowData data) {
  s_instance->m_windows.emplace_back(std::make_unique<Window>(std::move(data)));
  s_instance->m_windows.back()->init();
  if (s_instance->m_windows.size() == 1) {
    s_instance->m_windows.back()->make_context_current();
  }
  return *s_instance->m_windows.back();
}

void WindowManager::destroy_window(Window& window) {
  s_instance->m_windows.erase(std::remove_if(s_instance->m_windows.begin(), s_instance->m_windows.end(),
                                             [&window](const auto& w) { return *w == window; }));
}

void WindowManager::glfw_error_callback(int error, const char* description) {
  MGE_ERROR("GLFW:{} {}", error, description);
}

void WindowManager::terminate() {
  std::vector<std::unique_ptr<Window>> empty_vector;
  m_windows.swap(empty_vector);
  glfwTerminate();
  s_instance = nullptr;

  MGE_INFO("Window manager terminated");
}

WindowManager::WindowManager() {}

WindowManager::~WindowManager() {
  if (s_instance) {
    terminate();
  }
}

void WindowManager::init() {
  if (glfwInit() == GLFW_FALSE) {
    throw std::runtime_error("GLFW cannot be initialized");
  }
  glfwSetErrorCallback(WindowManager::glfw_error_callback);

  MGE_INFO("Window manager initialized");
}
}  // namespace mge
