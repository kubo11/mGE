#ifndef MGE_EVENTS_EVENT_HH
#define MGE_EVENTS_EVENT_HH

#include "../mgepch.hh"

namespace mge {
template <typename T>
class Event {
 public:
  Event() = default;
  Event(T type, const std::string& name = "") : m_type(type), m_name(name), m_handled(false) {}
  virtual ~Event() {}
  inline const T get_type() const { return m_type; }

  template <typename EventType>
  inline EventType to_type() const {
    return static_cast<const EventType&>(*this);
  }

  inline const std::string& get_name() const { return m_name; }
  virtual bool is_handled() const { return m_handled; }

 protected:
  T m_type;
  std::string m_name;
  bool m_handled;
};

template <typename T>
class EventDispatcher {
  using Func = std::function<bool(Event<T>&)>;

 public:
  int add_listener(T type, Func& func) {
    m_listeners[type].push_back(func);
    int handle = m_next_listener_id++;
    m_listener_handles[handle] = {type, std::prev(m_listeners[type].end())};
    return handle;
  }

  void remove_listener(int handle) {
    auto it = m_listener_handles.find(handle);
    if (it != m_listener_handles.end()) {
      const auto& listener_info = it->second;
      auto& listeners = m_listeners[listener_info.first];
      listeners.erase(listener_info.second);
      m_listener_handles.erase(it);
    }
  }

  void send_event(Event<T>& event) {
    if (m_listeners.find(event.get_type()) == m_listeners.end()) return;

    for (auto&& listener : m_listeners.at(event.get_type())) {
      if (!event.is_handled()) listener(event);
    }
  }

 private:
  std::map<T, std::vector<Func>> m_listeners;
  int m_next_listener_id = 0;
  std::map<int, std::pair<T, typename std::vector<Func>::iterator>> m_listener_handles;
};
}  // namespace mge

#endif  // MGE_EVENTS_EVENT_HH