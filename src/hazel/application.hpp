#pragma once

#include "hazel/events/application_event.hpp"
#include "hazel/layer_stack.hpp"
#include "hazel/window.hpp"

namespace hazel
{
class HZ_API Application
{
public:
  Application();
  virtual ~Application();

  virtual void run();

  virtual void on_event(Event& e);

  void push_layer(Layer* layer);
  void push_overlay(Layer* layer);

private:
  virtual bool on_window_close(WindowCloseEvent& e);
  std::unique_ptr<Window> window_;
  bool running_ = true;
  LayerStack layer_stack_; 
};

extern std::unique_ptr<Application> create_application();

} // namespace hazel
