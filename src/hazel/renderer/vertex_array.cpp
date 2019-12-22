#include "hazel/core.hpp"
#include "hazel/renderer/renderer.hpp"
#include "hazel/renderer/vertex_array.hpp"
#include "hazel/platform/opengl/opengl_vertex_array.hpp"

namespace hazel
{

std::unique_ptr<VertexArray> VertexArray::create() {
  switch (Renderer::api()) {
  case RendererAPI::API::None:
    HZ_CORE_ASSERT(false, "RendererAPI::None not supported");
    return nullptr;

  case RendererAPI::API::OpenGL:
    return std::make_unique<OpenGLVertexArray>();
  }

  HZ_CORE_ASSERT(false, "unknown RendererAPI");
  return nullptr;
}

} // namespace hazel
