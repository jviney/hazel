#include <iostream>

#include "hazel/hazel.hpp"

class Sandbox : public hazel::Application {
public:
  Sandbox() {}

  ~Sandbox() {}

  void run() override {}
};

std::unique_ptr<hazel::Application> hazel::create_application() {
  return std::make_unique<Sandbox>();
}
