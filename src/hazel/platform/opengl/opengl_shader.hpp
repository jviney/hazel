#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "hazel/renderer/shader.hpp"

namespace hazel
{

class OpenGLShader : public Shader
{
public:
  OpenGLShader(const std::string& filepath);
  OpenGLShader(const std::string& name, const std::string& vertex_source,
               const std::string& fragment_source);
  ~OpenGLShader();

  void bind() const override;
  void unbind() const override;

  const std::string& name() const override;

  void upload_uniform_int(const std::string& name, int) const;

  void upload_uniform_float(const std::string& name, float) const;
  void upload_uniform_float2(const std::string& name, const glm::vec2&) const;
  void upload_uniform_float3(const std::string& name, const glm::vec3&) const;
  void upload_uniform_float4(const std::string& name, const glm::vec4&) const;

  void upload_uniform_mat3(const std::string& name, const glm::mat3&) const;
  void upload_uniform_mat4(const std::string& name, const glm::mat4&) const;

private:
  std::unordered_map<GLenum, std::string> pre_process(const std::string& source);
  void compile(const std::unordered_map<GLenum, std::string>& shader_sources);
  std::string read_file(const std::string& filepath);

  uint32_t renderer_id_{};
  std::string name_;
};

} // namespace hazel
