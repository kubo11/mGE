#ifndef MGE_EVENTS_EVENT_HH
#define MGE_EVENTS_EVENT_HH

#include "../mgepch.hh"

#define BIND_EVENT_HANDLER(obj, ...) \
  std::bind(&obj, this, std::placeholders::_1, ##__VA_ARGS__)

namespace mge {
template <typename T>
using EventHandler = std::function<bool(T&)>;

class Event {
 public:
  bool handled = false;
  virtual ~Event() {}

  virtual inline const std::string name() const { return "Event"; }

  template <typename T>
  static bool try_handler(Event& event, EventHandler<T> handler) {
    T* specific_event = nullptr;
    if (nullptr != (specific_event = dynamic_cast<T*>(&event))) {
      event.handled = handler(*specific_event);
      return true;
    }

    return false;
  }
};
}  // namespace mge

#endif  // MGE_EVENTS_EVENT_HH