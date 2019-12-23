#include "hazel/core.hpp"
#include "hazel/log.hpp"
#include "hazel/renderer/renderer.hpp"
#include "hazel/renderer/shader.hpp"
#include "hazel/platform/opengl/opengl_shader.hpp"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace hazel
{

Scope<Shader> Shader::create(const std::string& vertex_source, const std::string& fragment_source) {
  switch (Renderer::api()) {
  case RendererAPI::API::None:
    HZ_CORE_ASSERT(false, "RendererAPI::None not supported");
    return nullptr;

  case RendererAPI::API::OpenGL:
    return std::make_unique<OpenGLShader>(vertex_source, fragment_source);
  }

  HZ_CORE_ASSERT(false, "unknown RendererAPI");
  return nullptr;
}

} // namespace hazel
