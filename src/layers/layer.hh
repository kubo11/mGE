#ifndef MGE_LAYERS_LAYER_HH
#define MGE_LAYERS_LAYER_HH

#include "../events/event.hh"

namespace mge {
class Layer {
 public:
  Layer(std::function<void(Event&)> send_event_callback)
      : m_send_event_callback(send_event_callback){};
  virtual ~Layer();
  virtual void attach();
  virtual void update();
  virtual void handle_event(Event& event, float dt);

 protected:
  std::function<void(Event&)> m_send_event_callback;
};
}  // namespace mge

#endif  // MGE_LAYERS_LAYER_HH