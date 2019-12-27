#pragma once

#include "hazel/core/application.hpp"

int main(int, char**) {
  hazel::Log::init();
  HZ_CORE_WARN("initialized log");
  int a = 5;
  HZ_INFO("hello var={0}", a);

  auto app = hazel::create_application();
  app->run();
}
