#include <iostream>

#include <imgui.h>

#include "hazel/events/event.hpp"
#include "hazel/events/key_event.hpp"
#include "hazel/hazel.hpp"

class ExampleLayer : public hazel::Layer
{
public:
  ExampleLayer() : Layer("Example"), camera_(-1.6f, 1.6f, -0.9f, 0.9f) {
    // clang-format off
    float vertices[] = {
      -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
      0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
      0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
    };
    // clang-format on

    std::shared_ptr<hazel::VertexBuffer> vertex_buffer =
        hazel::VertexBuffer::create(vertices, sizeof(vertices));

    hazel::BufferLayout layout = {{hazel::ShaderDataType::Float3, "a_position"},
                                  {hazel::ShaderDataType::Float4, "a_color"}};

    vertex_buffer->set_layout(layout);

    // Index buffer
    uint32_t indices[3] = {0, 1, 2};
    std::shared_ptr<hazel::IndexBuffer> index_buffer =
        hazel::IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t));

    vertex_array_ = hazel::VertexArray::create();
    vertex_array_->add_vertex_buffer(vertex_buffer);
    vertex_array_->set_index_buffer(index_buffer);

    // Square
    square_va_ = hazel::VertexArray::create();

    // clang-format off
    float square_vertices[] = {
      -0.75f, -0.75f, 0.0f,
      0.75f, -0.75f, 0.0f,
      0.75f,  0.75f, 0.0f,
      -0.75f,  0.75f, 0.0f,
    };
    // clang-format on

    std::shared_ptr<hazel::VertexBuffer> square_vb =
        hazel::VertexBuffer::create(square_vertices, sizeof(square_vertices));
    square_vb->set_layout({{hazel::ShaderDataType::Float3, "a_position"}});
    square_va_->add_vertex_buffer(square_vb);

    uint32_t square_indices[] = {0, 1, 2, 2, 3, 0};
    std::shared_ptr<hazel::IndexBuffer> square_ib =
        hazel::IndexBuffer::create(square_indices, sizeof(square_indices) / sizeof(uint32_t));
    square_va_->set_index_buffer(square_ib);

    auto vertex_source = R"(
      #version 330 core

      layout(location = 0) in vec3 a_position;
      layout(location = 1) in vec4 a_color;

      uniform mat4 u_view_projection;

      out vec3 v_position;
      out vec4 v_color;

      void main() {
        v_position = a_position;
        v_color = a_color;
        gl_Position = u_view_projection * vec4(a_position, 1.0);
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

    shader_ = std::make_unique<hazel::Shader>(vertex_source, fragment_source);

    auto blue_shader_vertex_source = R"(
      #version 330 core

      layout(location = 0) in vec3 a_position;

      uniform mat4 u_view_projection;

      out vec3 v_position;

      void main() {
        v_position = a_position;
        gl_Position = u_view_projection * vec4(a_position, 1.0);
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

    blue_shader_ =
        std::make_unique<hazel::Shader>(blue_shader_vertex_source, blue_shader_fragment_source);
  }

  void on_update() override {
    if (hazel::Input::is_key_pressed(HZ_KEY_LEFT)) {
      camera_position_.x += camera_speed_;
    } else if (hazel::Input::is_key_pressed(HZ_KEY_RIGHT)) {
      camera_position_.x -= camera_speed_;
    }

    if (hazel::Input::is_key_pressed(HZ_KEY_UP)) {
      camera_position_.y -= camera_speed_;
    } else if (hazel::Input::is_key_pressed(HZ_KEY_DOWN)) {
      camera_position_.y += camera_speed_;
    }

    if (hazel::Input::is_key_pressed(HZ_KEY_A)) {
      camera_rotation_ += camera_rotation_speed_;
    } else if (hazel::Input::is_key_pressed(HZ_KEY_D)) {
      camera_rotation_ -= camera_rotation_speed_;
    }

    hazel::RenderCommand::set_clear_color({0.25, 0.2f, 0.2f, 1.0f});
    hazel::RenderCommand::clear();

    camera_.set_position(camera_position_);
    camera_.set_rotation(camera_rotation_);

    hazel::Renderer::begin_scene(camera_);

    hazel::Renderer::submit(blue_shader_.get(), square_va_.get());
    hazel::Renderer::submit(shader_.get(), vertex_array_.get());

    hazel::Renderer::end_scene();
  }

  void on_imgui_render() override {}

  void on_event(hazel::Event& event) override {}

private:
  std::shared_ptr<hazel::VertexArray> vertex_array_;
  std::shared_ptr<hazel::Shader> shader_;

  std::shared_ptr<hazel::VertexArray> square_va_;
  std::shared_ptr<hazel::Shader> blue_shader_;

  hazel::OrthographicCamera camera_;
  glm::vec3 camera_position_{0.0f};
  float camera_speed_ = 0.1f;

  float camera_rotation_ = 0.0f;
  float camera_rotation_speed_ = 1.0f;
};

class Sandbox : public hazel::Application
{
public:
  Sandbox() { push_layer(std::make_unique<ExampleLayer>()); }

  ~Sandbox() {}

  // void run() override {}
};

std::unique_ptr<hazel::Application> hazel::create_application() {
  return std::make_unique<Sandbox>();
}
