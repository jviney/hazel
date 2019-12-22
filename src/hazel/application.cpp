#include "hazel/log.hpp"
#include "hazel/core.hpp"
#include "hazel/application.hpp"
#include "hazel/events/application_event.hpp"
#include "hazel/input.hpp"

#include <glad/glad.h>

namespace hazel
{

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

Application* Application::instance_ = nullptr;

Application::Application() {
  HZ_CORE_ASSERT(!Application::instance_, "application already exists");
  instance_ = this;

  window_ = Window::create();
  window_->set_event_callback(BIND_EVENT_FN(on_event));

  auto imgui_layer = std::make_unique<ImGuiLayer>();
  imgui_layer_ = imgui_layer.get();
  push_overlay(std::move(imgui_layer));

  // Triangle
  glGenVertexArrays(1, &vertex_array_);
  glBindVertexArray(vertex_array_);

  glGenBuffers(1, &vertex_buffer_);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);

  // clang-format off
  float vertices[3 * 3] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
  };
  // clang-format on

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

  // Index buffer
  glGenBuffers(1, &index_buffer_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_);

  unsigned int indices[3] = {0, 1, 2};
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  auto vertex_source = R"(
    #version 330 core

    layout(location = 0) in vec3 a_position;

    out vec3 v_position;

    void main() {
      v_position = a_position;
      gl_Position = vec4(a_position, 1.0);
    }
  )";

  auto fragment_source = R"(
    #version 330 core

    layout(location = 0) out vec4 color;

    in vec3 v_position;

    void main() {
      color = vec4(v_position * 0.5 + 0.5, 1.0);
    }
  )";

  shader_ = std::make_unique<Shader>(vertex_source, fragment_source);
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
    glClearColor(0.2f, 0.2f, 0.2f, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    shader_->bind();
    glBindVertexArray(vertex_array_);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

    for (auto& layer : layer_stack_) {
      layer->on_update();
    }

    imgui_layer_->begin();
    for (auto& layer : layer_stack_) {
      layer->on_imgui_render();
    }
    imgui_layer_->end();

    window_->on_update();
  }
}

bool Application::on_window_close(WindowCloseEvent&) {
  running_ = false;
  return true;
}

} // namespace hazel
