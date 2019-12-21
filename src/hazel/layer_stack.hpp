#pragma once

#include "hazel/layer.hpp"

namespace hazel
{
class HZ_API LayerStack
{
public:
  LayerStack();
  ~LayerStack();

  void push_layer(std::unique_ptr<Layer> layer);
  void push_overlay(std::unique_ptr<Layer> overlay);
  void pop_layer(std::unique_ptr<Layer> layer);
  void pop_overlay(std::unique_ptr<Layer> overlay);

  auto begin() { return layers_.begin(); }
  auto end() { return layers_.end(); }

private:
  std::vector<std::unique_ptr<Layer>> layers_;
  unsigned int layer_insert_index_ = 0;
};

} // namespace hazel
