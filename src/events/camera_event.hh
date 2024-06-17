#ifndef MGE_EVENTS_CAMERA_EVENT_HH
#define MGE_EVENTS_CAMERA_EVENT_HH

#include "event.hh"

namespace mge {
enum class CameraEvents { CameraAngleChanged, CameraPositionChanged, CameraZoom };

class CameraAngleChangedEvent : public Event<CameraEvents> {
 public:
  CameraAngleChangedEvent() : Event<CameraEvents>(CameraEvents::CameraAngleChanged, "CameraAngleChangedEvent") {}
  virtual ~CameraAngleChangedEvent() = default;
  float azimuth;
  float elevation;
};

class CameraPositionChangedEvent : public Event<CameraEvents> {
 public:
  CameraPositionChangedEvent()
      : Event<CameraEvents>(CameraEvents::CameraPositionChanged, "CameraPositionChangedEvent") {}
  virtual ~CameraPositionChangedEvent() = default;
  glm::vec2 pos;
};

class CameraZoomEvent : public Event<CameraEvents> {
 public:
  CameraZoomEvent() : Event<CameraEvents>(CameraEvents::CameraZoom, "CameraAngleChangedEvent") {}
  virtual ~CameraZoomEvent() = default;
  float zoom;
};
}  // namespace mge

#endif  // MGE_EVENTS_CAMERA_EVENT_HH