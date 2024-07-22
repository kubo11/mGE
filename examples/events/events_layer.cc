#include "events_layer.hh"

#include "events.hh"

#include <filesystem>
#include <random>
namespace fs = std::filesystem;

void EventsLayer::configure() {
  auto vertex_buffer = std::make_unique<mge::Buffer<FigureVertex>>();
  vertex_buffer->bind();
  vertex_buffer->copy(m_shapes[m_current_shape_idx]);
  vertex_buffer->unbind();
  auto vertex_array =
      std::make_unique<mge::VertexArray<FigureVertex>>(std::move(vertex_buffer), FigureVertex::get_attributes());
  auto shader_program = mge::ShaderSystem::acquire(fs::current_path() / "examples" / "events" / "shaders" / "figure");
  shader_program->set_uniform_value("color", glm::vec3{1.0f, 0.0f, 0.0f});
  mge::RenderPipelineBuilder pipeline_builder;
  m_render_pipeline = std::move(
      pipeline_builder.add_shader_program(shader_program).build<FigureVertex>(mge::DrawPrimitiveType::TRIANGLE));
  m_figure = std::move(std::make_unique<mge::RenderableComponent<FigureVertex>>(
      std::move(mge::RenderPipelineMap<FigureVertex>{{mge::RenderMode::SOLID, *m_render_pipeline}}),
      mge::RenderMode::SOLID, std::move(vertex_array)));
  mge::AddEventListener(mge::MouseEvents::MouseButtonPressed, EventsLayer::on_mouse_pressed, this);
  AddEventListener(FigureEvents::ShapeChanged, EventsLayer::on_figure_shape_changed, this);
  AddEventListener(FigureEvents::ColorChanged, EventsLayer::on_figure_color_changed, this);
}

void EventsLayer::update() { m_render_pipeline->run(); }

bool EventsLayer::on_mouse_pressed(mge::MouseButtonPressedEvent& event) {
  static std::default_random_engine generator;
  static std::uniform_real_distribution distribution(0.0f, 1.0f);
  if (event.button == mge::MouseButton::Left) {
    FigureColorChangedEvent color_event({distribution(generator), distribution(generator), distribution(generator)});
    SendEvent(color_event);
    return true;
  } else if (event.button == mge::MouseButton::Right) {
    m_current_shape_idx = 1 - m_current_shape_idx;
    FigureShapeChangedEvent shape_event(m_shapes[m_current_shape_idx]);
    SendEvent(shape_event);
    return true;
  }
  return false;
}

bool EventsLayer::on_figure_shape_changed(FigureShapeChangedEvent& event) {
  auto& vertex_buffer = m_figure->get_vertex_array().get_vertex_buffer();
  vertex_buffer.bind();
  vertex_buffer.copy(event.shape);
  vertex_buffer.unbind();
  return true;
}

bool EventsLayer::on_figure_color_changed(FigureColorChangedEvent& event) {
  m_render_pipeline->dynamic_uniform_update<glm::vec3>("color", [&event]() { return event.color; });
  return true;
}