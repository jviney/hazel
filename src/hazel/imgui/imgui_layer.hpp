#pragma once

#include "hazel/events/event.hpp"
#include "hazel/layer.hpp"

namespace hazel
{

class HZ_API ImGuiLayer : public Layer
{
public:
  ImGuiLayer();
  ~ImGuiLayer();

  void on_attach();
  void on_detach();
  void on_update();
  void on_event(Event& event);

private:
  float time_ = 0.0f;
};

} // namespace hazel
