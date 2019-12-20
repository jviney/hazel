#include "hazel/layer_stack.hpp"

namespace hazel
{

LayerStack::LayerStack() { layer_insert_ = layers_.begin(); }

LayerStack::~LayerStack() {}

void LayerStack::push_layer(std::unique_ptr<Layer> layer) {
  layer_insert_ = layers_.emplace(layer_insert_, std::move(layer));
}

void LayerStack::push_overlay(std::unique_ptr<Layer> layer) {
  layers_.emplace_back(std::move(layer));
}

void LayerStack::pop_layer(std::unique_ptr<Layer> layer) {
  auto it = std::find(layers_.begin(), layers_.end(), layer);

  if (it != layers_.end()) {
    layers_.erase(it);
    layer_insert_--;
  }
}

void LayerStack::pop_overlay(std::unique_ptr<Layer> overlay) {
  auto it = std::find(layers_.begin(), layers_.end(), overlay);

  if (it != layers_.end()) {
    layers_.erase(it);
  }
}

} // namespace hazel
