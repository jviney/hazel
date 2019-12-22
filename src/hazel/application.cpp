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

static GLenum shader_data_type_to_opengl_base_type(ShaderDataType type) {
  switch (type) {
  case ShaderDataType::None:
    HZ_CORE_ASSERT(false, "unknown ShaderDataType");
    return 0;

  case ShaderDataType::Float:
  case ShaderDataType::Float2:
  case ShaderDataType::Float3:
  case ShaderDataType::Float4:
  case ShaderDataType::Mat3:
  case ShaderDataType::Mat4:
    return GL_FLOAT;

  case ShaderDataType::Int:
  case ShaderDataType::Int2:
  case ShaderDataType::Int3:
  case ShaderDataType::Int4:
    return GL_INT;

  case ShaderDataType::Bool:
    return GL_BOOL;
  }
}

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

  // clang-format off
  float vertices[] = {
    -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
     0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
     0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
  };
  // clang-format on

  vertex_buffer_ = VertexBuffer::create(vertices, sizeof(vertices));

  {
    BufferLayout layout = {{ShaderDataType::Float3, "a_position"},
                           {ShaderDataType::Float4, "a_color"}};

    vertex_buffer_->set_layout(layout);
  }

  uint32_t index = 0;
  auto const& layout = vertex_buffer_->layout();
  for (const auto& element : layout) {
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, element.component_count(),
                          shader_data_type_to_opengl_base_type(element.type),
                          element.normalized ? GL_TRUE : GL_FALSE, layout.stride(),
                          (const void*) (uintptr_t) element.offset);
    index++;
  }

  // Index buffer
  uint32_t indices[3] = {0, 1, 2};
  index_buffer_ = IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t));

  auto vertex_source = R"(
    #version 330 core

    layout(location = 0) in vec3 a_position;
    layout(location = 1) in vec4 a_color;

    out vec3 v_position;
    out vec4 v_color;

    void main() {
      v_position = a_position;
      v_color = a_color;
      gl_Position = vec4(a_position, 1.0);
    }
  )";

  auto fragment_source = R"(
    #version 330 core

    layout(location = 0) out vec4 color;

    in vec3 v_position;
    in vec4 v_color;

    void main() {
      color = vec4(v_position * 0.5 + 0.5, 1.0);
      color = v_color;
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
    glDrawElements(GL_TRIANGLES, index_buffer_->count(), GL_UNSIGNED_INT, nullptr);

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
