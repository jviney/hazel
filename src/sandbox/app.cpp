#include <iostream>

#include "hazel/hazel.hpp"
#include "hazel/imgui/imgui_layer.hpp"

class ExampleLayer : public hazel::Layer
{
public:
  ExampleLayer() : Layer("Example") {}

  void on_update() override {}

  void on_event(hazel::Event& event) override { HZ_TRACE("{}", event); }
};

class Sandbox : public hazel::Application
{
public:
  Sandbox() {
    push_layer(std::make_unique<ExampleLayer>());
    push_overlay(std::make_unique<hazel::ImGuiLayer>());
  }

  ~Sandbox() {}

  // void run() override {}
};

std::unique_ptr<hazel::Application> hazel::create_application() {
  return std::make_unique<Sandbox>();
}
