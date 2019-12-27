#include <imgui.h>
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "sandbox_2d.hpp"
#include "hazel/core/entry_point.hpp"

Sandbox2D::Sandbox2D() : Layer("Sandbox 2D"), camera_controller_(16.0f / 9.0f) {}

void Sandbox2D::on_attach() {
  checkerboard_texture_ = hazel::Texture2D::create("../assets/textures/checkerboard.png");
}

void Sandbox2D::on_detach() {}

void Sandbox2D::on_update(hazel::Timestep ts) {
  camera_controller_.on_update(ts);

  hazel::RenderCommand::set_clear_color({0.25, 0.2f, 0.2f, 1.0f});
  hazel::RenderCommand::clear();

  hazel::Renderer2D::begin_scene(camera_controller_.camera());
  hazel::Renderer2D::draw_quad({-1.0f, 0.0f}, {0.8f, 0.8f}, {0.8f, 0.2f, 0.3f, 1.0f});
  hazel::Renderer2D::draw_quad({0.5f, -0.5f}, {0.5f, 0.75f}, {0.2f, 0.3f, 0.8f, 1.0f});
  hazel::Renderer2D::draw_quad({0.0f, 0.0f, -0.1f}, {10.0f, 10.0f}, checkerboard_texture_);
  hazel::Renderer2D::end_scene();
}

void Sandbox2D::on_imgui_render() {
  ImGui::Begin("Settings");
  ImGui::ColorEdit4("Square color", glm::value_ptr(square_color_));
  ImGui::End();
}

void Sandbox2D::on_event(hazel::Event& event) { camera_controller_.on_event(event); }
