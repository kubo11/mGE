#ifndef MGE_EVENTS_LIGHT_EVENT_HH
#define MGE_EVENTS_LIGHT_EVENT_HH

#include "../renderer/light.hh"
#include "event.hh"

namespace mge {
class LightEvent : public Event {
 public:
  LightEvent(const Light& light) : m_light(light) {}
  virtual inline const std::string name() const override {
    return "LightEvent";
  }
  inline const Light& get_light() const { return m_light; }

 protected:
  const Light& m_light;
};

class LightAddEvent : public LightEvent {
 public:
  LightAddEvent(const Light& light) : LightEvent(light) {}
  virtual inline const std::string name() const override {
    return "LightAddEvent";
  }
};

class LightRemoveEvent : public LightEvent {
 public:
  LightRemoveEvent(const Light& light) : LightEvent(light) {}
  virtual inline const std::string name() const override {
    return "LightRemoveEvent";
  }
};

class LightUpdateEvent : public LightEvent {
 public:
  LightUpdateEvent(const Light& light) : LightEvent(light) {}
  virtual inline const std::string name() const override {
    return "LightUpdateEvent";
  }
};
}  // namespace mge

#endif  // MGE_EVENTS_LIGHT_EVENT_HH