#include <iostream>
#include <vector>

#include <imgui.h>
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "hazel/events/application_event.hpp"
#include "hazel/platform/opengl/opengl_shader.hpp"
#include "hazel/hazel.hpp"
#include "hazel/renderer/texture.hpp"

#include "sandbox_2d.hpp"

class ExampleLayer : public hazel::Layer
{
public:
  ExampleLayer() : Layer("Example"), camera_controller_(16.0f / 9.0f) {
    auto a = std::vector<int>(1);
    auto b = std::vector<int>{1, 2, 3};
    std::vector<int> c{1, 2, 3};
    // clang-format off
    float vertices[] = {
      -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
      0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
      0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
    };
    // clang-format on

    hazel::Ref<hazel::VertexBuffer> vertex_buffer =
        hazel::VertexBuffer::create(vertices, sizeof(vertices));

    hazel::BufferLayout layout = {{hazel::ShaderDataType::Float3, "a_position"},
                                  {hazel::ShaderDataType::Float4, "a_color"}};

    vertex_buffer->set_layout(layout);

    // Index buffer
    uint32_t indices[3] = {0, 1, 2};
    hazel::Ref<hazel::IndexBuffer> index_buffer =
        hazel::IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t));

    vertex_array_ = hazel::VertexArray::create();
    vertex_array_->add_vertex_buffer(vertex_buffer);
    vertex_array_->set_index_buffer(index_buffer);

    // Square
    square_va_ = hazel::VertexArray::create();

    // clang-format off
    float square_vertices[] = {
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
       0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
       0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
      -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
    };
    // clang-format on

    hazel::Ref<hazel::VertexBuffer> square_vb =
        hazel::VertexBuffer::create(square_vertices, sizeof(square_vertices));
    square_vb->set_layout({{hazel::ShaderDataType::Float3, "a_position"},
                           {hazel::ShaderDataType::Float2, "a_tex_coord"}});
    square_va_->add_vertex_buffer(square_vb);

    uint32_t square_indices[] = {0, 1, 2, 2, 3, 0};
    hazel::Ref<hazel::IndexBuffer> square_ib =
        hazel::IndexBuffer::create(square_indices, sizeof(square_indices) / sizeof(uint32_t));
    square_va_->set_index_buffer(square_ib);

    auto vertex_source = R"(
      #version 330 core

      layout(location = 0) in vec3 a_position;
      layout(location = 1) in vec4 a_color;

      uniform mat4 u_view_projection;
      uniform mat4 u_transform;

      out vec3 v_position;
      out vec4 v_color;

      void main() {
        v_position = a_position;
        v_color = a_color;
        gl_Position = u_view_projection * u_transform * vec4(a_position, 1.0);
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

    shader_ = hazel::Shader::create("vertex position color", vertex_source, fragment_source);

    auto flat_color_shader_vertex_source = R"(
      #version 330 core

      layout(location = 0) in vec3 a_position;

      uniform mat4 u_view_projection;
      uniform mat4 u_transform;

      out vec3 v_position;

      void main() {
        v_position = a_position;
        gl_Position = u_view_projection * u_transform * vec4(a_position, 1.0);
      }
    )";

    auto flat_color_shader_fragment_source = R"(
      #version 330 core

      layout(location = 0) out vec4 color;

      in vec3 v_position;

      uniform vec3 u_color;

      void main() {
        color = vec4(u_color, 1.0);
      }
    )";

    flat_color_shader_ = hazel::Shader::create("flat color shader", flat_color_shader_vertex_source,
                                               flat_color_shader_fragment_source);

    auto texture_shader = shader_library_.load("../assets/shaders/texture.glsl");

    texture_ = hazel::Texture2D::create("../assets/textures/checkerboard.png");
    cherno_logo_texture_ = hazel::Texture2D::create("../assets/textures/cherno_logo.png");

    std::dynamic_pointer_cast<hazel::OpenGLShader>(texture_shader)->bind();
    std::dynamic_pointer_cast<hazel::OpenGLShader>(texture_shader)
        ->upload_uniform_int("u_texture", 0);
  }

  void on_update(hazel::Timestep ts) override {
    camera_controller_.on_update(ts);

    hazel::RenderCommand::set_clear_color({0.25, 0.2f, 0.2f, 1.0f});
    hazel::RenderCommand::clear();

    hazel::Renderer::begin_scene(camera_controller_.camera());

    auto scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

    std::dynamic_pointer_cast<hazel::OpenGLShader>(flat_color_shader_)->bind();
    std::dynamic_pointer_cast<hazel::OpenGLShader>(flat_color_shader_)
        ->upload_uniform_float3("u_color", square_color_);

    for (int y = 0; y < 20; y++) {
      for (int x = 0; x < 20; x++) {
        auto position = glm::vec3(x * 0.11f, y * 0.11f, 0.0f);
        auto transform = glm::translate(glm::mat4(1.0f), position) * scale;

        hazel::Renderer::submit(flat_color_shader_.get(), square_va_.get(), transform);
      }
    }

    auto texture_shader = shader_library_.get("texture");

    texture_->bind();
    hazel::Renderer::submit(texture_shader.get(), square_va_.get(),
                            glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

    cherno_logo_texture_->bind();
    hazel::Renderer::submit(texture_shader.get(), square_va_.get(),
                            glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

    // Triangle
    // hazel::Renderer::submit(shader_.get(), vertex_array_.get());

    hazel::Renderer::end_scene();
  }

  void on_imgui_render() override {
    ImGui::Begin("Settings");
    ImGui::ColorEdit3("Square color", glm::value_ptr(square_color_));
    ImGui::End();
  }

  void on_event(hazel::Event& event) override { camera_controller_.on_event(event); }

private:
  hazel::ShaderLibrary shader_library_;

  hazel::Ref<hazel::Shader> shader_;
  hazel::Ref<hazel::VertexArray> vertex_array_;

  hazel::Ref<hazel::VertexArray> square_va_;
  hazel::Ref<hazel::Shader> flat_color_shader_;

  hazel::OrthographicCameraController camera_controller_;

  glm::vec3 square_color_{0.2f, 0.3f, 0.8f};

  hazel::Ref<hazel::Texture2D> texture_;
  hazel::Ref<hazel::Texture2D> cherno_logo_texture_;
};

class Sandbox : public hazel::Application
{
public:
  Sandbox() {
    // push_layer(std::make_unique<ExampleLayer>());
    push_layer(std::make_unique<Sandbox2D>());
  }

  ~Sandbox() {}

  // void run() override {}
};

hazel::Scope<hazel::Application> hazel::create_application() { return std::make_unique<Sandbox>(); }
