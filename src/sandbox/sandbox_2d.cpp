#include <imgui.h>
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "hazel/platform/opengl/opengl_shader.hpp"

#include "sandbox_2d.hpp"
#include "hazel/core/entry_point.hpp"

Sandbox2D::Sandbox2D() : Layer("Sandbox 2D"), camera_controller_(16.0f / 9.0f) {}

void Sandbox2D::on_attach() {}

void Sandbox2D::on_detach() {}

void Sandbox2D::on_update(hazel::Timestep ts) {
  camera_controller_.on_update(ts);

  hazel::RenderCommand::set_clear_color({0.25, 0.2f, 0.2f, 1.0f});
  hazel::RenderCommand::clear();

  hazel::Renderer2D::begin_scene(camera_controller_.camera());
  hazel::Renderer2D::draw_quad({0.0f, 0.0f}, {1.0f, 1.0f}, {0.8f, 0.2f, 0.3f, 1.0f});
  hazel::Renderer2D::end_scene();

  // flat_color_shader_->bind();

  // TODO: Add Shader::set_mat4 and Shader::set_float4
  // std::dynamic_pointer_cast<hazel::OpenGLShader>(flat_color_shader_)->upload_uniform_float4("u_color",
  // square_color_);
}

void Sandbox2D::on_imgui_render() {
  ImGui::Begin("Settings");
  ImGui::ColorEdit4("Square color", glm::value_ptr(square_color_));
  ImGui::End();
}

void Sandbox2D::on_event(hazel::Event& event) { camera_controller_.on_event(event); }
