#pragma once

#include "hazel/layer.hpp"

namespace hazel
{
class HZ_API LayerStack
{
public:
  LayerStack();
  ~LayerStack();

  void push_layer(Layer* layer);
  void push_overlay(Layer* layer);
  void pop_layer(Layer* layer);
  void pop_overlay(Layer* overlay);

  auto begin() { return layers_.begin(); }
  auto end() { return layers_.end(); }

private:
  std::vector<Layer*> layers_;
  std::vector<Layer*>::iterator layer_insert_;
};

} // namespace hazel
