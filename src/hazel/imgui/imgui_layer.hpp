#pragma once

#include "hazel/events/application_event.hpp"
#include "hazel/events/key_event.hpp"
#include "hazel/events/mouse_event.hpp"
#include "hazel/layer.hpp"

namespace hazel
{

class ImGuiLayer : public Layer
{
public:
  ImGuiLayer();
  ~ImGuiLayer();

  void on_attach() override;
  void on_detach() override;
  void on_imgui_render() override;

  void begin();
  void end();
};

} // namespace hazel
