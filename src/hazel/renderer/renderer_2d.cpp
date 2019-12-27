#include "hazel/core/core.hpp"
#include "hazel/renderer/render_command.hpp"
#include "hazel/renderer/renderer_2d.hpp"
#include "hazel/renderer/shader.hpp"
#include "hazel/renderer/vertex_array.hpp"
#include "hazel/platform/opengl/opengl_shader.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace hazel
{

struct Renderer2DStorage {
  Scope<VertexArray> quad_vertex_array;
  Scope<Shader> texture_shader;
  Scope<Texture> white_texture;
};

static Scope<Renderer2DStorage> s_data;

void Renderer2D::init() {
  s_data = std::make_unique<Renderer2DStorage>();

  s_data->quad_vertex_array = VertexArray::create();

  // clang-format off
  float square_vertices[] = {
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
     0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
     0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
    -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
  };
  // clang-format on

  Ref<VertexBuffer> square_vertex_buffer =
      VertexBuffer::create(square_vertices, sizeof(square_vertices));
  square_vertex_buffer->set_layout(
      {{ShaderDataType::Float3, "a_position"}, {ShaderDataType::Float2, "a_tex_coord"}});
  s_data->quad_vertex_array->add_vertex_buffer(square_vertex_buffer);

  uint32_t square_indices[] = {0, 1, 2, 2, 3, 0};
  Ref<IndexBuffer> square_index_buffer_ =
      IndexBuffer::create(square_indices, sizeof(square_indices) / sizeof(uint32_t));
  s_data->quad_vertex_array->set_index_buffer(square_index_buffer_);

  s_data->white_texture = Texture2D::create(1, 1);
  uint32_t white_texture_data = 0xffffffff;
  s_data->white_texture->set_data(&white_texture_data, sizeof(white_texture_data));

  s_data->texture_shader = Shader::create("../assets/shaders/texture.glsl");
  s_data->texture_shader->set_int("t_texture", 0);
}

void Renderer2D::shutdown() { s_data.reset(); }

void Renderer2D::begin_scene(const OrthographicCamera& camera) {
  s_data->texture_shader->bind();
  s_data->texture_shader->set_mat4("u_view_projection", camera.view_projection_matrix());
}

void Renderer2D::end_scene() {}

void Renderer2D::draw_quad(const glm::vec2& position, const glm::vec2& size,
                           const glm::vec4& color) {
  draw_quad({position.x, position.y, 0.0f}, size, color);
}

void Renderer2D::draw_quad(const glm::vec3& position, const glm::vec2& size,
                           const glm::vec4& color) {
  s_data->texture_shader->bind();
  s_data->texture_shader->set_float4("u_color", color);
  s_data->white_texture->bind();

  auto transform = glm::translate(glm::mat4(1.0f), position) *
                   glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
  s_data->texture_shader->set_mat4("u_transform", transform);

  s_data->quad_vertex_array->bind();
  RenderCommand::draw_indexed(s_data->quad_vertex_array.get());
}

void Renderer2D::draw_quad(const glm::vec2& position, const glm::vec2& size,
                           const Ref<Texture2D>& texture) {
  draw_quad({position.x, position.y, 1.0f}, size, texture);
}

void Renderer2D::draw_quad(const glm::vec3& position, const glm::vec2& size,
                           const Ref<Texture2D>& texture) {
  s_data->texture_shader->bind();
  s_data->texture_shader->set_float4("u_color", glm::vec4(1.0f));

  auto transform = glm::translate(glm::mat4(1.0f), position) *
                   glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
  s_data->texture_shader->set_mat4("u_transform", transform);

  texture->bind();

  s_data->quad_vertex_array->bind();
  RenderCommand::draw_indexed(s_data->quad_vertex_array.get());
}

} // namespace hazel
