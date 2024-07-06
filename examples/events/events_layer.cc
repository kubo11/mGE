#include "events_layer.hh"

#include "events.hh"

#include <filesystem>
#include <random>
namespace fs = std::filesystem;

EventsLayer::EventsLayer()
    : m_shader(mge::ShaderSystem::acquire(fs::current_path() / "examples" / "events" / "shaders" / "figure")),
      m_vertex_buffer(std::make_unique<mge::Buffer<FigureVertex>>()),
      m_vertex_array(std::make_unique<mge::VertexArray>()) {}

void EventsLayer::configure() {
  m_vertex_buffer->bind();
  m_vertex_buffer->copy(m_shapes[m_current_shape_idx]);
  m_vertex_buffer->unbind();
  m_vertex_array->bind();
  m_vertex_array->attach_buffer(*m_vertex_buffer.get(), FigureVertex::get_attributes());
  m_vertex_array->unbind();
  m_shader->set_uniform_value("color", glm::vec3{1.0f, 0.0f, 0.0f});
  mge::AddEventListener(mge::WindowEvents::WindowMousePressed, EventsLayer::on_mouse_pressed, this);
  AddEventListener(FigureEvents::ShapeChanged, EventsLayer::on_figure_shape_changed, this);
  AddEventListener(FigureEvents::ColorChanged, EventsLayer::on_figure_color_changed, this);
}

void EventsLayer::update() {
  m_shader->bind();
  m_vertex_array->bind();
  glDrawArrays(GL_TRIANGLES, 0, m_vertex_buffer->get_count());
  m_vertex_array->unbind();
  m_shader->unbind();
}

bool EventsLayer::on_mouse_pressed(mge::WindowMousePressedEvent& event) {
  static std::default_random_engine generator;
  static std::uniform_real_distribution distribution(0.0f, 1.0f);
  if (event.button == GLFW_MOUSE_BUTTON_LEFT) {
    FigureColorChangedEvent color_event({distribution(generator), distribution(generator), distribution(generator)});
    SendEvent(color_event);
    return true;
  } else if (event.button == GLFW_MOUSE_BUTTON_RIGHT) {
    m_current_shape_idx = 1 - m_current_shape_idx;
    FigureShapeChangedEvent shape_event(m_shapes[m_current_shape_idx]);
    SendEvent(shape_event);
    return true;
  }
  return false;
}

bool EventsLayer::on_figure_shape_changed(FigureShapeChangedEvent& event) {
  m_vertex_buffer->bind();
  m_vertex_buffer->copy(event.shape);
  m_vertex_buffer->unbind();
  return true;
}

bool EventsLayer::on_figure_color_changed(FigureColorChangedEvent& event) {
  m_shader->set_uniform_value("color", event.color);
  return true;
}