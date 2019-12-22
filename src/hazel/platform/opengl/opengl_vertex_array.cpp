#include "hazel/core.hpp"
#include "hazel/platform/opengl/opengl_vertex_array.hpp"

#include <glad/glad.h>

namespace hazel
{

static GLenum shader_data_type_to_opengl_base_type(ShaderDataType type) {
  switch (type) {
  case ShaderDataType::None:
    HZ_CORE_ASSERT(false, "unknown ShaderDataType");
    return 0;

  case ShaderDataType::Float:
  case ShaderDataType::Float2:
  case ShaderDataType::Float3:
  case ShaderDataType::Float4:
  case ShaderDataType::Mat3:
  case ShaderDataType::Mat4:
    return GL_FLOAT;

  case ShaderDataType::Int:
  case ShaderDataType::Int2:
  case ShaderDataType::Int3:
  case ShaderDataType::Int4:
    return GL_INT;

  case ShaderDataType::Bool:
    return GL_BOOL;
  }
}

OpenGLVertexArray::OpenGLVertexArray() {
  glGenVertexArrays(1, &renderer_id_);
  glBindVertexArray(renderer_id_);
}

OpenGLVertexArray::~OpenGLVertexArray() { glDeleteVertexArrays(1, &renderer_id_); }

void OpenGLVertexArray::bind() const { glBindVertexArray(renderer_id_); }

void OpenGLVertexArray::unbind() const { glBindVertexArray(0); }

void OpenGLVertexArray::add_vertex_buffer(std::shared_ptr<VertexBuffer> vertex_buffer) {
  HZ_CORE_ASSERT(vertex_buffer->layout().elements().size(), "vertex buffer has no layout");

  glBindVertexArray(renderer_id_);
  vertex_buffer->bind();

  uint32_t index = 0;
  auto const& layout = vertex_buffer->layout();
  for (const auto& element : layout) {
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, element.component_count(),
                          shader_data_type_to_opengl_base_type(element.type),
                          element.normalized ? GL_TRUE : GL_FALSE, layout.stride(),
                          (const void*) (uintptr_t) element.offset);
    index++;
  }

  vertex_buffers_.push_back(std::move(vertex_buffer));
}

void OpenGLVertexArray::set_index_buffer(std::shared_ptr<IndexBuffer> index_buffer) {
  glBindVertexArray(renderer_id_);
  index_buffer->bind();

  index_buffer_ = std::move(index_buffer);
}

} // namespace hazel
