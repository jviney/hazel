#include <imgui.h>
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "hazel/platform/opengl/opengl_shader.hpp"

#include "sandbox_2d.hpp"
#include "hazel/core/entry_point.hpp"

Sandbox2D::Sandbox2D() : Layer("Sandbox 2D"), camera_controller_(16.0f / 9.0f) {}

void Sandbox2D::on_attach() {
  // Square
  square_vertex_array_ = hazel::VertexArray::create();

  // clang-format off
    float square_vertices[] = {
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
       0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
       0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
      -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
    };
  // clang-format on

  hazel::Ref<hazel::VertexBuffer> square_vertex_buffer =
      hazel::VertexBuffer::create(square_vertices, sizeof(square_vertices));
  square_vertex_buffer->set_layout({{hazel::ShaderDataType::Float3, "a_position"},
                                    {hazel::ShaderDataType::Float2, "a_tex_coord"}});
  square_vertex_array_->add_vertex_buffer(square_vertex_buffer);

  uint32_t square_indices[] = {0, 1, 2, 2, 3, 0};
  hazel::Ref<hazel::IndexBuffer> square_index_buffer_ =
      hazel::IndexBuffer::create(square_indices, sizeof(square_indices) / sizeof(uint32_t));
  square_vertex_array_->set_index_buffer(square_index_buffer_);

  flat_color_shader_ = hazel::Shader::create("../assets/shaders/flat_color.glsl");
}

void Sandbox2D::on_detach() {}

void Sandbox2D::on_update(hazel::Timestep ts) {
  camera_controller_.on_update(ts);

  hazel::RenderCommand::set_clear_color({0.25, 0.2f, 0.2f, 1.0f});
  hazel::RenderCommand::clear();

  hazel::Renderer::begin_scene(camera_controller_.camera());

  auto scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

  flat_color_shader_->bind();
  std::dynamic_pointer_cast<hazel::OpenGLShader>(flat_color_shader_)
      ->upload_uniform_float4("u_color", square_color_);

  hazel::Renderer::submit(flat_color_shader_.get(), square_vertex_array_.get());

  hazel::Renderer::end_scene();
}

void Sandbox2D::on_imgui_render() {
  ImGui::Begin("Settings");
  ImGui::ColorEdit4("Square color", glm::value_ptr(square_color_));
  ImGui::End();
}

void Sandbox2D::on_event(hazel::Event& event) { camera_controller_.on_event(event); }
