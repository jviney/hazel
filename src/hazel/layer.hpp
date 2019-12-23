#pragma once

#include "hazel/core.hpp"
#include "hazel/core/timestep.hpp"
#include "hazel/events/event.hpp"

namespace hazel
{
class Layer
{
public:
  Layer(const std::string& debug_name = "Layer");
  virtual ~Layer();

  virtual void on_attach() {}
  virtual void on_detach() {}
  virtual void on_update(Timestep) {}
  virtual void on_event(Event& /*event */) {}
  virtual void on_imgui_render() {}

  const std::string& name() const { return debug_name_; }

protected:
  std::string debug_name_;
};
} // namespace hazel
