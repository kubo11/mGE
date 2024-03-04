#ifndef MGE_EVENTS_RAYCASTER_EVENT_HH
#define MGE_EVENTS_RAYCASTER_EVENT_HH

#include "event.hh"

namespace mge {
class RaycasterEvent : public Event {
 public:
  RaycasterEvent() {}
  virtual inline const std::string name() const override {
    return "RaycasterEvent";
  }
};

class RaycasterResetEvent : public RaycasterEvent {
 public:
  RaycasterResetEvent() : RaycasterEvent() {}
  virtual inline const std::string name() const override {
    return "RaycasterResetEvent";
  }
};

class RaycasterUpdateMaxPixelSizeExponentEvent : public RaycasterEvent {
 public:
  RaycasterUpdateMaxPixelSizeExponentEvent(uint8_t max_pixel_size_exponent)
      : m_max_pixel_size_exponent(max_pixel_size_exponent) {}
  virtual inline const std::string name() const override {
    return "RaycasterUpdateMaxPixelSizeExponentEvent";
  }
  inline const uint8_t get_max_pixel_size_exponent() const {
    return m_max_pixel_size_exponent;
  }

 private:
  uint8_t m_max_pixel_size_exponent;
};
}  // namespace mge

#endif  // MGE_EVENTS_RAYCASTER_EVENT_HH