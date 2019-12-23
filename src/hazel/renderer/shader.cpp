#include "hazel/core.hpp"
#include "hazel/log.hpp"
#include "hazel/renderer/renderer.hpp"
#include "hazel/renderer/shader.hpp"
#include "hazel/platform/opengl/opengl_shader.hpp"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace hazel
{

Scope<Shader> Shader::create(const std::string& filepath) {
  switch (Renderer::api()) {
  case RendererAPI::API::None:
    HZ_CORE_ASSERT(false, "RendererAPI::None not supported");
    return nullptr;

  case RendererAPI::API::OpenGL:
    return std::make_unique<OpenGLShader>(filepath);
  }

  HZ_CORE_ASSERT(false, "unknown RendererAPI");
  return nullptr;
}

Scope<Shader> Shader::create(const std::string& name, const std::string& vertex_source,
                             const std::string& fragment_source) {
  switch (Renderer::api()) {
  case RendererAPI::API::None:
    HZ_CORE_ASSERT(false, "RendererAPI::None not supported");
    return nullptr;

  case RendererAPI::API::OpenGL:
    return std::make_unique<OpenGLShader>(name, vertex_source, fragment_source);
  }

  HZ_CORE_ASSERT(false, "unknown RendererAPI");
  return nullptr;
}

void ShaderLibrary::add(Ref<Shader> shader) {
  auto& name = shader->name();
  add(name, std::move(shader));
}

void ShaderLibrary::add(const std::string& name, Ref<Shader> shader) {
  HZ_CORE_ASSERT(exists(name), "shader already exists");

  shaders_[name] = std::move(shader);
}

Ref<Shader> ShaderLibrary::load(const std::string& filepath) {
  Ref<Shader> shader = Shader::create(filepath);
  add(shader);
  return shader;
}

Ref<Shader> ShaderLibrary::load(const std::string& name, const std::string& filepath) {
  Ref<Shader> shader = Shader::create(filepath);
  add(name, shader);
  return shader;
}

Ref<Shader> ShaderLibrary::get(const std::string& name) {
  HZ_CORE_ASSERT(!exists(name), "shader not found");

  return shaders_[name];
}

bool ShaderLibrary::exists(const std::string& name) const {
  return shaders_.find(name) != shaders_.end();
}

} // namespace hazel
