#ifndef MGE_EVENTS_FIGURE_EVENT
#define MGE_EVENTS_FIGURE_EVENT

#include "mge.hh"

#include "figure_vertex.hh"

enum class FigureEvents { ShapeChanged, ColorChanged };

class FigureShapeChangedEvent : public mge::Event<FigureEvents> {
 public:
  FigureShapeChangedEvent(const std::vector<FigureVertex>& shape)
      : mge::Event<FigureEvents>(FigureEvents::ShapeChanged, "FigureShapeChangedEvent"), shape(shape) {}
  virtual ~FigureShapeChangedEvent() = default;
  std::vector<FigureVertex> shape;
};

class FigureColorChangedEvent : public mge::Event<FigureEvents> {
 public:
  FigureColorChangedEvent(const glm::vec3& color)
      : mge::Event<FigureEvents>(FigureEvents::ColorChanged, "FigureColorChangedEvent"), color(color) {}
  virtual ~FigureColorChangedEvent() = default;
  glm::vec3 color;
};

#endif  // MGE_EVENTS_FIGURE_EVENT