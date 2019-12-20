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

  inline static auto& get() { return *instance_; }

  void push_layer(std::unique_ptr<Layer> layer);
  void push_overlay(std::unique_ptr<Layer> layer);

  inline auto& window() { return window_; }

private:
  virtual bool on_window_close(WindowCloseEvent& e);
  std::unique_ptr<Window> window_;
  bool running_ = true;
  LayerStack layer_stack_;

  static Application* instance_;
};

extern std::unique_ptr<Application> create_application();

} // namespace hazel
