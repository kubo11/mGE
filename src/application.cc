#include "application.hh"

namespace mge {
Application::Application(const ApplicationParams& config) : m_running(true), m_name(config.name) {
  m_logger = Logger::create();
  m_timer = std::make_unique<Timer>();
  m_event_manager = EventManager::create(*m_timer);
  m_window_manager = WindowManager::create();
  m_main_window = WindowManager::create_window(WindowData(config.name, config.window_width, config.window_height));
  m_render_context = RenderContext::create();
  m_shader_system = ShaderSystem::create();
  WindowManager::get_instance().set_swap_interval(0);
  RenderContext::get_instance().set_viewport_dims(0, 0, config.window_width, config.window_height);
  m_ui_manager = UIManager::create(*m_main_window);

  m_layer_stack = std::make_unique<LayerStack>();
  m_scene = std::make_unique<mge::Scene>(std::move(std::make_unique<mge::Camera>(
      glm::vec3{3.0f, 3.0f, 0.0f}, 180, -45, 45,
      static_cast<float>(config.window_width) / static_cast<float>(config.window_height), 0.01f, 100.0f)));

  AddEventListener(WindowEvents::WindowClosed, Application::on_window_closed, this);

  MGE_INFO("Application \"{}\" created", m_main_window->get_title());
}

Application::~Application() {
  if (m_running) terminate();
}

void Application::run() {
  while (m_running) {
    m_main_window->clear();

    for (auto& layer : *m_layer_stack) {
      layer->update();
    }

    m_main_window->update();
    m_timer->tick();
  }
}

void Application::terminate() {
  m_layer_stack->terminate();
  m_ui_manager->terminate();
  m_shader_system->terminate();
  WindowManager::destroy_window(*m_main_window);
  m_main_window = nullptr;
  m_window_manager->terminate();
  m_render_context->terminate();
  m_event_manager->terminate();
  m_timer = nullptr;

  MGE_INFO("Application \"{}\" terminated", m_name);

  m_logger->terminate();
}

void Application::push_layer(std::unique_ptr<Layer> layer) {
  layer->configure();
  m_layer_stack->push(std::move(layer));
}

bool Application::on_window_closed(WindowClosedEvent& event) {
  if (event.window == *m_main_window) {
    m_running = false;
    MGE_INFO("Applicaiton \"{}\" stopping", m_name);
    return true;
  }
  return false;
}
}  // namespace mge
