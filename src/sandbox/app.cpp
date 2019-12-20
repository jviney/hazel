#include <iostream>

#include "hazel/hazel.hpp"

class ExampleLayer : public hazel::Layer
{
public:
  ExampleLayer() : Layer("Example") {}

  void on_update() override { HZ_INFO("ExampleLayer::Update"); }

  void on_event(hazel::Event& event) override { HZ_TRACE("{}", event); }
};

class Sandbox : public hazel::Application
{
public:
  Sandbox() { push_layer(new ExampleLayer()); }

  ~Sandbox() {}

  // void run() override {}
};

std::unique_ptr<hazel::Application> hazel::create_application() {
  return std::make_unique<Sandbox>();
}
