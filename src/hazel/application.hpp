#pragma once

#include "hazel/events/application_event.hpp"
#include "hazel/imgui/imgui_layer.hpp"
#include "hazel/layer_stack.hpp"
#include "hazel/renderer/buffer.hpp"
#include "hazel/renderer/shader.hpp"
#include "hazel/renderer/vertex_array.hpp"
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

  void push_layer(std::unique_ptr<Layer> layer);
  void push_overlay(std::unique_ptr<Layer> layer);

  inline auto& window() { return *window_; }

private:
  virtual bool on_window_close(WindowCloseEvent& e);
  std::unique_ptr<Window> window_;
  ImGuiLayer* imgui_layer_;
  bool running_ = true;
  LayerStack layer_stack_;

  std::shared_ptr<VertexArray> vertex_array_;
  std::shared_ptr<Shader> shader_;

  std::shared_ptr<VertexArray> square_va_;
  std::shared_ptr<Shader> blue_shader_;

  static Application* instance_;
};

extern std::unique_ptr<Application> create_application();

} // namespace hazel
