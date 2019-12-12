#include "hazel/core.hpp"

#include "hazel/application.hpp"
#include "hazel/events/application_event.hpp"

namespace hazel
{

Application::Application() { window_ = std::unique_ptr<Window>(Window::create()); }

Application::~Application() {}

void Application::run() {
  while (running_) {
    window_->on_update();
  }
}

} // namespace hazel
