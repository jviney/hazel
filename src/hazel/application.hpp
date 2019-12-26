#pragma once

#include <chrono>

#include "hazel/core/timestep.hpp"
#include "hazel/events/application_event.hpp"
#include "hazel/imgui/imgui_layer.hpp"
#include "hazel/layer_stack.hpp"
#include "hazel/window.hpp"

namespace hazel
{
class Application
{
public:
  Application();
  virtual ~Application();

  virtual void run();

  virtual void on_event(Event& e);

  inline static auto& get() { return *instance_; }

  void push_layer(Scope<Layer> layer);
  void push_overlay(Scope<Layer> layer);

  inline auto& window() { return *window_; }

private:
  virtual bool on_window_close(WindowCloseEvent& e);
  virtual bool on_window_resize(WindowResizeEvent& e);

  Scope<Window> window_;
  ImGuiLayer* imgui_layer_;
  bool running_ = true;
  bool minimized_ = false;
  LayerStack layer_stack_;
  Timestep timestep_;
  std::chrono::time_point<std::chrono::steady_clock> last_frame_time_ =
      std::chrono::steady_clock::now();

  static Application* instance_;
};

extern Scope<Application> create_application();

} // namespace hazel
