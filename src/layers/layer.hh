#ifndef MGE_LAYERS_LAYER_HH
#define MGE_LAYERS_LAYER_HH

#include "../events/event.hh"

namespace mge {
class Layer {
 public:
  Layer() : m_send_event() {}
  virtual ~Layer() {}
  virtual void configure() = 0;
  virtual void update() = 0;
  virtual void handle_event(Event& event, float dt) = 0;
  inline virtual void set_event_handler(
      std::function<void(Event&)> send_event) final {
    m_send_event = send_event;
  }

 protected:
  std::function<void(Event&)> m_send_event;
};
}  // namespace mge

#endif  // MGE_LAYERS_LAYER_HH