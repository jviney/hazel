#include "hazel/core/core.hpp"
#include "hazel/platform/opengl/opengl_shader.hpp"
#include "hazel/renderer/renderer.hpp"
#include "hazel/renderer/vertex_array.hpp"

namespace hazel
{

Renderer::SceneData* Renderer::scene_data_ = new SceneData();

void Renderer::init() { RenderCommand::init(); }

void Renderer::on_window_resize(uint32_t width, uint32_t height) {
  RenderCommand::set_viewport(0, 0, width, height);
}

void Renderer::begin_scene(OrthographicCamera& camera) {
  scene_data_->view_projection_matrix = camera.view_projection_matrix();
}

void Renderer::end_scene() {}

void Renderer::submit(const Shader* shader, const VertexArray* vertex_array,
                      const glm::mat4& transform) {
  auto opengl_shader = static_cast<const OpenGLShader*>(shader);

  opengl_shader->bind();

  opengl_shader->upload_uniform_mat4("u_view_projection", scene_data_->view_projection_matrix);
  opengl_shader->upload_uniform_mat4("u_transform", transform);

  vertex_array->bind();

  RenderCommand::draw_indexed(vertex_array);
}

} // namespace hazel
