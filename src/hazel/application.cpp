#include "hazel/log.hpp"
#include "hazel/core.hpp"
#include "hazel/application.hpp"
#include "hazel/events/application_event.hpp"
#include "hazel/input.hpp"
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

  auto imgui_layer = std::make_unique<ImGuiLayer>();
  imgui_layer_ = imgui_layer.get();
  push_overlay(std::move(imgui_layer));

  // clang-format off
  float vertices[] = {
    -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
     0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
     0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
  };
  // clang-format on

  std::shared_ptr<VertexBuffer> vertex_buffer = VertexBuffer::create(vertices, sizeof(vertices));

  BufferLayout layout = {{ShaderDataType::Float3, "a_position"},
                         {ShaderDataType::Float4, "a_color"}};

  vertex_buffer->set_layout(layout);

  // Index buffer
  uint32_t indices[3] = {0, 1, 2};
  std::shared_ptr<IndexBuffer> index_buffer =
      IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t));

  vertex_array_ = VertexArray::create();
  vertex_array_->add_vertex_buffer(vertex_buffer);
  vertex_array_->set_index_buffer(index_buffer);

  // Square
  square_va_ = VertexArray::create();

  // clang-format off
  float square_vertices[] = {
    -0.75f, -0.75f, 0.0f, 
     0.75f, -0.75f, 0.0f, 
     0.75f,  0.75f, 0.0f, 
    -0.75f,  0.75f, 0.0f, 
  };
  // clang-format on

  std::shared_ptr<VertexBuffer> square_vb =
      VertexBuffer::create(square_vertices, sizeof(square_vertices));
  square_vb->set_layout({{ShaderDataType::Float3, "a_position"}});
  square_va_->add_vertex_buffer(square_vb);

  uint32_t square_indices[] = {0, 1, 2, 2, 3, 0};
  std::shared_ptr<IndexBuffer> square_ib =
      IndexBuffer::create(square_indices, sizeof(square_indices) / sizeof(uint32_t));
  square_va_->set_index_buffer(square_ib);

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

  auto blue_shader_vertex_source = R"(
    #version 330 core

    layout(location = 0) in vec3 a_position;

    out vec3 v_position;

    void main() {
      v_position = a_position;
      gl_Position = vec4(a_position, 1.0);
    }
  )";

  auto blue_shader_fragment_source = R"(
    #version 330 core

    layout(location = 0) out vec4 color;

    in vec3 v_position;

    void main() {
      color = vec4(0.2, 0.3, 0.8, 1.0);
    }
  )";

  blue_shader_ = std::make_unique<Shader>(blue_shader_vertex_source, blue_shader_fragment_source);
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
    RenderCommand::set_clear_color({0.25, 0.2f, 0.2f, 1.0f});
    RenderCommand::clear();

    Renderer::begin_scene();

    blue_shader_->bind();
    Renderer::submit(square_va_.get());

    shader_->bind();
    Renderer::submit(vertex_array_.get());

    Renderer::end_scene();

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
