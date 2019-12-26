#include "hazel/core/core.hpp"
#include "hazel/renderer/render_command.hpp"
#include "hazel/renderer/renderer_2d.hpp"
#include "hazel/renderer/shader.hpp"
#include "hazel/renderer/vertex_array.hpp"
#include "hazel/platform/opengl/opengl_shader.hpp"

namespace hazel
{

struct Renderer2DStorage {
  Scope<VertexArray> quad_vertex_array;
  Scope<Shader> flat_color_shader;
};

static Scope<Renderer2DStorage> s_data;

void Renderer2D::init() {
  s_data = std::make_unique<Renderer2DStorage>();

  s_data->quad_vertex_array = hazel::VertexArray::create();

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
  s_data->quad_vertex_array->add_vertex_buffer(square_vertex_buffer);

  uint32_t square_indices[] = {0, 1, 2, 2, 3, 0};
  hazel::Ref<hazel::IndexBuffer> square_index_buffer_ =
      hazel::IndexBuffer::create(square_indices, sizeof(square_indices) / sizeof(uint32_t));
  s_data->quad_vertex_array->set_index_buffer(square_index_buffer_);

  s_data->flat_color_shader = hazel::Shader::create("../assets/shaders/flat_color.glsl");
}

void Renderer2D::shutdown() { s_data.reset(); }

void Renderer2D::begin_scene(const OrthographicCamera& camera) {
  auto* shader = dynamic_cast<OpenGLShader*>(s_data->flat_color_shader.get());
  shader->bind();
  shader->upload_uniform_mat4("u_view_projection", camera.view_projection_matrix());
  shader->upload_uniform_mat4("u_transform", glm::mat4(1.0f));
}

void Renderer2D::end_scene() {}

void Renderer2D::draw_quad(const glm::vec2& position, const glm::vec2& size,
                           const glm::vec4& color) {
  draw_quad({position.x, position.y, 0.0f}, size, color);
}

void Renderer2D::draw_quad(const glm::vec3& position, const glm::vec2& size,
                           const glm::vec4& color) {
  auto* shader = dynamic_cast<OpenGLShader*>(s_data->flat_color_shader.get());
  shader->bind();
  shader->upload_uniform_float4("u_color", color);

  s_data->quad_vertex_array->bind();
  RenderCommand::draw_indexed(s_data->quad_vertex_array.get());
}

} // namespace hazel
