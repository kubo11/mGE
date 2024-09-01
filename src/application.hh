#ifndef MGE_APPLICATION_HH
#define MGE_APPLICATION_HH

#include "mgepch.hh"

#include "events/event.hh"
#include "events/event_manager.hh"
#include "events/events.hh"
#include "layers/layer_stack.hh"
#include "render_context.hh"
#include "renderer/camera/camera.hh"
#include "renderer/scene/scene.hh"
#include "shader_system.hh"
#include "timer.hh"
#include "ui_manager.hh"
#include "window_manager.hh"

namespace mge {
struct ApplicationParams {
  uint16_t window_width = 1280;
  uint16_t window_height = 720;
  std::string name = "mGE applicaiton";
};

class Application {
 public:
  virtual ~Application();

  PREVENT_COPY(Application);

  void run();
  void terminate();
  void push_layer(std::unique_ptr<Layer> layer);

  inline Scene& get_scene() { return *m_scene; }

  bool on_window_closed(WindowClosedEvent& event);

 protected:
  bool m_running;
  std::string m_name;
  std::unique_ptr<Timer> m_timer;
  std::unique_ptr<LayerStack> m_layer_stack;
  std::unique_ptr<Scene> m_scene;
  std::shared_ptr<Window> m_main_window;

  std::shared_ptr<Logger> m_logger;
  std::shared_ptr<EventManager> m_event_manager;
  std::shared_ptr<ShaderSystem> m_shader_system;
  std::shared_ptr<WindowManager> m_window_manager;
  std::shared_ptr<UIManager> m_ui_manager;
  std::shared_ptr<RenderContext> m_render_context;

  Application(const ApplicationParams& config = {});
};
}  // namespace mge

#endif  // MGE_APPLICATION_HH