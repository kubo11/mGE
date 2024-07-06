#ifndef MGE_EVENTS_LAYER
#define MGE_EVENTS_LAYER

#include "mge.hh"

#include "figure_event.hh"
#include "figure_vertex.hh"

class EventsLayer : public mge::Layer {
 public:
  EventsLayer();
  ~EventsLayer() = default;

  virtual void configure() override;
  virtual void update() override;

  bool on_mouse_pressed(mge::WindowMousePressedEvent& event);
  bool on_figure_shape_changed(FigureShapeChangedEvent& event);
  bool on_figure_color_changed(FigureColorChangedEvent& event);

 private:
  std::unique_ptr<mge::VertexArray> m_vertex_array = nullptr;
  std::unique_ptr<mge::Buffer<FigureVertex>> m_vertex_buffer = nullptr;
  std::shared_ptr<mge::ShaderProgram> m_shader = nullptr;
  unsigned int m_current_shape_idx = 0;
  const std::array<std::vector<FigureVertex>, 2> m_shapes = {
      {{{{-0.5f, -0.5f, 0.0f}}, {{0.5f, -0.5f, 0.0f}}, {{0.0f, 0.5f, 0.0f}}},
       {{{-0.5f, -0.5f, 0.0f}},
        {{0.5f, -0.5f, 0.0f}},
        {{0.5f, 0.5f, 0.0f}},
        {{0.5f, 0.5f, 0.0f}},
        {{-0.5f, 0.5f, 0.0f}},
        {{-0.5f, -0.5f, 0.0f}}}}};
};

#endif  // MGE_EVENTS_LAYER