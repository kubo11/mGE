#include "application.hh"

namespace mge {
Application::Application(const ApplicationParams& config)
    : m_running(true),
      m_layer_stack(),
      m_window_manager(WindowManager::create()),
      m_render_context(RenderContext::create()),
      m_main_window(WindowManager::create_window(
          WindowData(config.name, config.window_width, config.window_height))),
      m_timer(),
      m_logger(Logger::create()),
      m_name(config.name) {
  m_main_window.set_event_handler(BIND_EVENT_HANDLER(Application::send_event));
  glViewport(0, 0, m_main_window.get_width(), m_main_window.get_height());

  MGE_INFO("Application \"{}\" created", m_main_window.get_title());
}

Application::~Application() {
  m_layer_stack.terminate();
  WindowManager::destroy_window(m_main_window);
  m_window_manager.terminate();
  m_render_context.terminate();

  MGE_INFO("Application \"{}\" terminated", m_name);
}

void Application::run() {
  while (m_running) {
    for (auto& layer : m_layer_stack) {
      layer->update();
    }

    m_main_window.update();
  }
}

void Application::push_layer(std::unique_ptr<Layer> layer) {
  m_layer_stack.push(std::move(layer));
}

void Application::send_event(Event& event) {
  MGE_INFO("{} sent", event.name());

  Event::try_handler<WindowClosedEvent>(
      event, BIND_EVENT_HANDLER(Application::on_window_closed));

  for (auto& layer : m_layer_stack | std::views::reverse) {
    layer->handle_event(event, m_timer.get_dt());
    if (event.handled) {
      break;
    }
  }
}

bool Application::on_window_closed(WindowClosedEvent& event) {
  if (event.get_window() == m_main_window) {
    m_running = false;
    MGE_INFO("Applicaiton \"{}\" stopping", m_name);
    return true;
  }
  return false;
}
}  // namespace mge
