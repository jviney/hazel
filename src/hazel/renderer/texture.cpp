#include "hazel/core/core.hpp"
#include "hazel/platform/opengl/opengl_texture.hpp"
#include "hazel/renderer/renderer.hpp"
#include "hazel/renderer/texture.hpp"

namespace hazel
{

Scope<Texture2D> Texture2D::create(uint32_t width, uint32_t height) {
  switch (Renderer::api()) {
  case RendererAPI::API::None:
    HZ_CORE_ASSERT(false, "RendererAPI::None not supported");
    return nullptr;

  case RendererAPI::API::OpenGL:
    return std::make_unique<OpenGLTexture2D>(width, height);
  }

  HZ_CORE_ASSERT(false, "unknown RendererAPI");
  return nullptr;
}

Scope<Texture2D> Texture2D::create(const std::string& path) {
  switch (Renderer::api()) {
  case RendererAPI::API::None:
    HZ_CORE_ASSERT(false, "RendererAPI::None not supported");
    return nullptr;

  case RendererAPI::API::OpenGL:
    return std::make_unique<OpenGLTexture2D>(path);
  }

  HZ_CORE_ASSERT(false, "unknown RendererAPI");
  return nullptr;
}

} // namespace hazel
