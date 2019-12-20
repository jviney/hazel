#include "hazel/core.hpp"
#include "hazel/log.hpp"
#include "hazel/application.hpp"
#include "hazel/events/application_event.hpp"

namespace hazel
{

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

Application* Application::instance_ = nullptr;

Application::Application() {
  HZ_CORE_ASSERT(Application::instance_ == nullptr, "application already exists");
  instance_ = this;

  window_ = std::unique_ptr<Window>(Window::create());
  window_->set_event_callback(BIND_EVENT_FN(on_event));
}

Application::~Application() {}

void Application::push_layer(std::unique_ptr<Layer> layer) {
  layer->on_attach();
  layer_stack_.push_layer(std::move(layer));
}

void Application::push_overlay(std::unique_ptr<Layer> layer) {
  layer->on_attach();
  layer_stack_.push_overlay(std::move(layer));
}

void Application::on_event(Event& event) {
  HZ_CORE_TRACE("{}", event);

  auto dispatcher = EventDispatcher(event);
  dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(on_window_close));

  for (auto it = layer_stack_.end(); it != layer_stack_.begin();) {
    (*--it)->on_event(event);
    if (event.handled()) {
      break;
    }
  }
}

void Application::run() {
  while (running_) {
    for (auto& layer : layer_stack_) {
      layer->on_update();
    }

    window_->on_update();
  }
}

bool Application::on_window_close(WindowCloseEvent&) {
  running_ = false;
  return true;
}

} // namespace hazel
