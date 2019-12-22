#include "hazel/core.hpp"
#include "hazel/platform/opengl/opengl_renderer_api.hpp"

#include <glad/glad.h>

namespace hazel
{

void OpenGLRendererAPI::clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void OpenGLRendererAPI::set_clear_color(const glm::vec4& color) {
  glClearColor(color.r, color.g, color.b, color.a);
}

void OpenGLRendererAPI::draw_indexed(const VertexArray* vertex_array) {
  glDrawElements(GL_TRIANGLES, vertex_array->index_buffer()->count(), GL_UNSIGNED_INT, nullptr);
}

} // namespace hazel
