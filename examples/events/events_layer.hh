#ifndef MGE_EVENTS_LAYER
#define MGE_EVENTS_LAYER

#include "mge.hh"

#include "figure_event.hh"
#include "figure_vertex.hh"

class EventsLayer : public mge::Layer {
 public:
  EventsLayer() = default;
  ~EventsLayer() = default;

  virtual void configure() override;
  virtual void update() override;

  bool on_mouse_pressed(mge::MouseButtonUpdatedEvent& event);
  bool on_figure_shape_changed(FigureShapeChangedEvent& event);
  bool on_figure_color_changed(FigureColorChangedEvent& event);

 private:
  std::unique_ptr<mge::RenderableComponent<FigureVertex>> m_figure = nullptr;
  std::unique_ptr<mge::RenderPipeline<FigureVertex>> m_render_pipeline = nullptr;
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