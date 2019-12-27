#include "hazel/core/core.hpp"
#include "hazel/core/application.hpp"
#include "hazel/platform/opengl/opengl_renderer_api.hpp"

#include <glad/glad.h>

namespace hazel
{

void OpenGLRendererAPI::init() {
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_DEPTH_TEST);
}

void OpenGLRendererAPI::set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
  auto const& window = Application::get().window();
  glViewport(x, y, width * window.content_scale_x(), height * window.content_scale_y());
}

void OpenGLRendererAPI::clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void OpenGLRendererAPI::set_clear_color(const glm::vec4& color) {
  glClearColor(color.r, color.g, color.b, color.a);
}

void OpenGLRendererAPI::draw_indexed(const VertexArray* vertex_array) {
  glDrawElements(GL_TRIANGLES, vertex_array->index_buffer()->count(), GL_UNSIGNED_INT, nullptr);
}

} // namespace hazel
