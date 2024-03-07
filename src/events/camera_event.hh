#ifndef MGE_EVENTS_CAMERA_EVENT_HH
#define MGE_EVENTS_CAMERA_EVENT_HH

#include "event.hh"

namespace mge {
class CameraEvent : public Event {
 public:
  CameraEvent() {}
  virtual inline const std::string name() const override {
    return "CameraEvent";
  }
};

class CameraAngleEvent : public CameraEvent {
 public:
  CameraAngleEvent(float azimuth, float elevation)
      : m_azimuth(azimuth), m_elevation(elevation) {}
  virtual inline const std::string name() const override {
    return "CameraAngleEvent";
  }

  inline const float get_azimuth() const { return m_azimuth; }
  inline const float get_elevation() const { return m_elevation; }

 private:
  float m_azimuth;
  float m_elevation;
};

class CameraPositionEvent : public CameraEvent {
 public:
  CameraPositionEvent(glm::vec2 pos) : m_pos(pos) {}
  virtual inline const std::string name() const override {
    return "CameraPositionEvent";
  }

  inline const glm::vec2 get_pos() const { return m_pos; }

 private:
  glm::vec2 m_pos;
};

class CameraZoomEvent : public CameraEvent {
 public:
  CameraZoomEvent(float zoom) : m_zoom(zoom) {}
  virtual inline const std::string name() const override {
    return "CameraZoomEvent";
  }

  inline const float get_zoom() const { return m_zoom; }

 private:
  float m_zoom;
};
}  // namespace mge

#endif  // MGE_EVENTS_CAMERA_EVENT_HH