#ifndef MGE_LAYERS_LAYER_HH
#define MGE_LAYERS_LAYER_HH

#include "../events/event.hh"

namespace mge {
class Layer {
 public:
  Layer() {}
  virtual ~Layer() {}
  virtual void configure() = 0;
  virtual void update() = 0;
};
}  // namespace mge

#endif  // MGE_LAYERS_LAYER_HH