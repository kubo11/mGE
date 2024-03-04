#ifndef MGE_APPLICATION_HH
#define MGE_APPLICATION_HH

#include "mgepch.hh"

#include "events/event.hh"
#include "events/standard_events.hh"
#include "layers/layer_stack.hh"
#include "render_context.hh"
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
 protected:
  Logger m_logger;
  Timer m_timer;
  WindowManager& m_window_manager;
  Window& m_main_window;
  RenderContext& m_render_context;
  UIManager& m_ui_manager;
  LayerStack m_layer_stack;
  std::string m_name;

  bool m_running;

  bool on_window_closed(WindowClosedEvent& event);

 public:
  Application(const ApplicationParams& config = {});

  virtual ~Application() {}

  void run();
  void terminate();
  void push_layer(std::unique_ptr<Layer> layer);
  void send_event(Event& event);
};
}  // namespace mge

#endif  // MGE_APPLICATION_HH