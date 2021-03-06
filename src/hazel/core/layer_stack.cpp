#include "hazel/core/layer_stack.hpp"

namespace hazel
{

LayerStack::LayerStack() {}

LayerStack::~LayerStack() {}

void LayerStack::push_layer(Scope<Layer> layer) {
  layers_.emplace(layers_.begin() + layer_insert_index_, std::move(layer));
  layer_insert_index_++;
}

void LayerStack::push_overlay(Scope<Layer> layer) { layers_.emplace_back(std::move(layer)); }

void LayerStack::pop_layer(Scope<Layer> layer) {
  auto it = std::find(layers_.begin(), layers_.end(), layer);

  if (it != layers_.end()) {
    layers_.erase(it);
    layer_insert_index_--;
  }
}

void LayerStack::pop_overlay(Scope<Layer> overlay) {
  auto it = std::find(layers_.begin(), layers_.end(), overlay);

  if (it != layers_.end()) {
    layers_.erase(it);
  }
}

} // namespace hazel
