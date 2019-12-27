#pragma once

#include "hazel/hazel.hpp"

class Sandbox2D : public hazel::Layer
{
public:
  Sandbox2D();
  ~Sandbox2D() = default;

  void on_attach() override;
  void on_detach() override;
  void on_update(hazel::Timestep ts) override;
  void on_imgui_render() override;
  void on_event(hazel::Event& event) override;

private:
  hazel::OrthographicCameraController camera_controller_;

  hazel::Ref<hazel::VertexArray> square_vertex_array_;
  hazel::Ref<hazel::VertexBuffer> square_vertex_buffer_;
  hazel::Ref<hazel::IndexBuffer> square_index_buffer_;
  hazel::Ref<hazel::Shader> flat_color_shader_;
  hazel::Ref<hazel::Texture2D> checkerboard_texture_;

  glm::vec4 square_color_{0.2f, 0.3f, 0.8f, 1.0f};
};
