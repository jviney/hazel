#pragma once

#include "hazel/core/layer.hpp"

namespace hazel
{
class LayerStack
{
public:
  LayerStack();
  ~LayerStack();

  void push_layer(Scope<Layer> layer);
  void push_overlay(Scope<Layer> overlay);
  void pop_layer(Scope<Layer> layer);
  void pop_overlay(Scope<Layer> overlay);

  auto begin() { return layers_.begin(); }
  auto end() { return layers_.end(); }

private:
  std::vector<Scope<Layer>> layers_;
  unsigned int layer_insert_index_ = 0;
};

} // namespace hazel
