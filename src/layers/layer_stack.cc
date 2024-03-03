#include "layer_stack.hh"

namespace mge {
LayerStack::LayerStack() { MGE_INFO("Layer stack created"); }

void LayerStack::push(std::unique_ptr<Layer> layer) {
  m_layers.push_back(std::move(layer));
}

void LayerStack::pop() { m_layers.pop_back(); }

void LayerStack::terminate() {
  std::vector<std::unique_ptr<Layer>> empty_vector;
  m_layers.swap(empty_vector);

  MGE_INFO("Layer stack terminated");
}
}  // namespace mge
