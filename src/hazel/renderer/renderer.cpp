#include "hazel/core.hpp"
#include "hazel/renderer/renderer.hpp"
#include "hazel/renderer/vertex_array.hpp"

namespace hazel
{

void Renderer::begin_scene() {}

void Renderer::end_scene() {}

void Renderer::submit(VertexArray* vertex_array) {
  vertex_array->bind();
  RenderCommand::draw_indexed(vertex_array);
}

} // namespace hazel
