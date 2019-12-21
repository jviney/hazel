#include <iostream>

#include <imgui.h>

#include "hazel/hazel.hpp"
#include "hazel/imgui/imgui_layer.hpp"

class ExampleLayer : public hazel::Layer
{
public:
  ExampleLayer() : Layer("Example") {}

  void on_update() override {}

  void on_imgui_render() override {
    ImGui::Begin("Test");
    ImGui::TextUnformatted("Hello");
    ImGui::End();
  }

  void on_event(hazel::Event& event) override { HZ_TRACE("{}", event); }
};

class Sandbox : public hazel::Application
{
public:
  Sandbox() { push_layer(std::make_unique<ExampleLayer>()); }

  ~Sandbox() {}

  // void run() override {}
};

std::unique_ptr<hazel::Application> hazel::create_application() {
  return std::make_unique<Sandbox>();
}
