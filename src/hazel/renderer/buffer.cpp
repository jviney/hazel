#include "hazel/core/core.hpp"
#include "hazel/platform/opengl/opengl_buffer.hpp"
#include "hazel/renderer/buffer.hpp"
#include "hazel/renderer/renderer.hpp"

namespace hazel
{

Scope<VertexBuffer> VertexBuffer::create(float* vertices, uint32_t size) {
  switch (Renderer::api()) {
  case RendererAPI::API::None:
    HZ_CORE_ASSERT(false, "RendererAPI::None not supported");
    return nullptr;

  case RendererAPI::API::OpenGL:
    return std::make_unique<OpenGLVertexBuffer>(vertices, size);
  }

  HZ_CORE_ASSERT(false, "unknown RendererAPI");
  return nullptr;
}

Scope<IndexBuffer> IndexBuffer::create(uint32_t* vertices, uint32_t size) {
  switch (Renderer::api()) {
  case RendererAPI::API::None:
    HZ_CORE_ASSERT(false, "RendererAPI::None not supported");
    return nullptr;

  case RendererAPI::API::OpenGL:
    return std::make_unique<OpenGLIndexBuffer>(vertices, size);
  }

  HZ_CORE_ASSERT(false, "unknown RendererAPI");
  return nullptr;
}

} // namespace hazel
