#ifndef MGE_EVENTS_CAMERA_EVENT_HH
#define MGE_EVENTS_CAMERA_EVENT_HH

#include "event.hh"

namespace mge {
enum class CameraEvents { CameraAngleChanged, CameraPositionChanged, CameraZoom };

class CameraAngleChangedEvent : public Event<CameraEvents> {
 public:
  CameraAngleChangedEvent(float yaw, float pitch)
      : Event<CameraEvents>(CameraEvents::CameraAngleChanged, "CameraAngleChangedEvent"), yaw(yaw), pitch(pitch) {}
  virtual ~CameraAngleChangedEvent() = default;
  float yaw;
  float pitch;
};

class CameraPositionChangedEvent : public Event<CameraEvents> {
 public:
  CameraPositionChangedEvent(glm::vec2 offset)
      : Event<CameraEvents>(CameraEvents::CameraPositionChanged, "CameraPositionChangedEvent"), offset(offset) {}
  virtual ~CameraPositionChangedEvent() = default;
  glm::vec2 offset;
};

class CameraZoomEvent : public Event<CameraEvents> {
 public:
  CameraZoomEvent(float zoom) : Event<CameraEvents>(CameraEvents::CameraZoom, "CameraAngleChangedEvent"), zoom(zoom) {}
  virtual ~CameraZoomEvent() = default;
  float zoom;
};
}  // namespace mge

#endif  // MGE_EVENTS_CAMERA_EVENT_HH