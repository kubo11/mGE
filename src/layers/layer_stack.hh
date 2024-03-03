#ifndef MGE_LAYERS_LAYER_STACK_HH
#define MGE_LAYERS_LAYER_STACK_HH

#include "../mgepch.hh"

#include "layer.hh"

namespace mge {
class LayerStack {
 public:
  LayerStack();
  ~LayerStack() = default;

  void push(std::unique_ptr<Layer> layer);
  void pop();
  void terminate();

  std::vector<std::unique_ptr<Layer>>::iterator begin() {
    return m_layers.begin();
  }
  std::vector<std::unique_ptr<Layer>>::iterator end() { return m_layers.end(); }

 private:
  std::vector<std::unique_ptr<Layer>> m_layers;
};
}  // namespace mge

#endif  // MGE_LAYERS_LAYER_STACK_HH