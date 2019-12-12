#include "hazel/core.hpp"

#include "hazel/application.hpp"
#include "hazel/events/application_event.hpp"
#include "hazel/log.hpp"

namespace hazel
{

Application::Application() {}

Application::~Application() {}

void Application::run() {
  auto e = hazel::WindowResizeEvent(1280, 720);
  HZ_TRACE(e);

  while (true) {
  }
}

} // namespace hazel
