#include "hazel/core/core.hpp"
#include "hazel/core/log.hpp"
#include "hazel/core/timestep.hpp"
#include "hazel/core/application.hpp"
#include "hazel/events/application_event.hpp"
#include "hazel/core/input.hpp"
#include "hazel/renderer/renderer.hpp"

namespace hazel
{

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

Application* Application::instance_ = nullptr;

Application::Application() {
  HZ_CORE_ASSERT(!Application::instance_, "application already exists");
  instance_ = this;

  window_ = Window::create();
  window_->set_event_callback(BIND_EVENT_FN(on_event));

  Renderer::init();

  auto imgui_layer = std::make_unique<ImGuiLayer>();
  imgui_layer_ = imgui_layer.get();
  push_overlay(std::move(imgui_layer));
}

Application::~Application() {}

void Application::push_layer(Scope<Layer> layer) {
  layer->on_attach();
  layer_stack_.push_layer(std::move(layer));
}

void Application::push_overlay(Scope<Layer> layer) {
  layer->on_attach();
  layer_stack_.push_overlay(std::move(layer));
}

void Application::on_event(Event& event) {
  HZ_CORE_TRACE("{}", event);

  auto dispatcher = EventDispatcher(event);
  dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(on_window_close));
  dispatcher.dispatch<WindowResizeEvent>(BIND_EVENT_FN(on_window_resize));

  for (auto it = layer_stack_.end(); it != layer_stack_.begin();) {
    (*--it)->on_event(event);
    if (event.handled()) {
      break;
    }
  }
}

void Application::run() {
  while (running_) {
    auto time = std::chrono::steady_clock::now();
    auto diff = std::chrono::duration<float>(time - last_frame_time_).count();
    auto timestep = Timestep(diff);

    if (!minimized_) {
      for (auto& layer : layer_stack_) {
        layer->on_update(timestep);
      }
    }

    imgui_layer_->begin();
    for (auto& layer : layer_stack_) {
      layer->on_imgui_render();
    }
    imgui_layer_->end();

    window_->on_update();

    last_frame_time_ = time;
  }
}

bool Application::on_window_close(WindowCloseEvent&) {
  running_ = false;
  return true;
}

bool Application::on_window_resize(WindowResizeEvent& e) {
  if (e.width() == 0 || e.height() == 0) {
    minimized_ = true;
    return false;
  }

  minimized_ = false;
  Renderer::on_window_resize(e.width(), e.height());
  return false;
}

} // namespace hazel
