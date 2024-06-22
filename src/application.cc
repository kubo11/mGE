#include "application.hh"

namespace mge {
Application::Application(const ApplicationParams& config)
    : m_running(true),
      m_layer_stack(),
      m_main_window(WindowManager::create_window(WindowData(config.name, config.window_width, config.window_height))),
      m_timer(),
      m_name(config.name),
      m_scene(std::make_unique<Camera>(20.0f, 1280.0f / 720.0f, 0.1f, 1000.0f)) {
  glViewport(0, 0, m_main_window.get_width(), m_main_window.get_height());
  glfwSwapInterval(0);

  m_ui_manager = UIManager::create(m_main_window);

  MGE_INFO("Application \"{}\" created", m_main_window.get_title());
}

Application::~Application() {
  if (m_running) terminate();
}

std::unique_ptr<Application> Application::create(const ApplicationParams& config) {
  auto logger = Logger::create();
  auto event_manager = EventManager::create();
  auto window_manager = WindowManager::create();
  auto render_context = RenderContext::create();
  auto shader_system = ShaderSystem::create();
  auto app = std::unique_ptr<Application>(new Application(config));
  app->m_logger = logger;
  app->m_event_manager = event_manager;
  app->m_window_manager = window_manager;
  app->m_render_context = render_context;
  app->m_shader_system = shader_system;

  return app;
}

void Application::run() {
  while (m_running) {
    m_main_window.clear();

    for (auto& layer : m_layer_stack) {
      layer->update();
    }

    m_main_window.update();
  }
}

void Application::terminate() {
  m_layer_stack.terminate();
  m_ui_manager->terminate();
  m_shader_system->terminate();
  WindowManager::destroy_window(m_main_window);
  m_window_manager->terminate();
  m_render_context->terminate();
  m_event_manager->terminate();
  m_logger->terminate();

  MGE_INFO("Application \"{}\" terminated", m_name);
}

void Application::push_layer(std::unique_ptr<Layer> layer) {
  layer->configure();
  m_layer_stack.push(std::move(layer));
}

bool Application::on_window_closed(WindowClosedEvent& event) {
  if (event.window == m_main_window) {
    m_running = false;
    MGE_INFO("Applicaiton \"{}\" stopping", m_name);
    return true;
  }
  return false;
}
}  // namespace mge
