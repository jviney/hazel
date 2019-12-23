#pragma once

#include <glm/glm.hpp>

namespace hazel
{

class Shader
{
public:
  Shader(const std::string& vertex_source, const std::string& fragment_source);
  ~Shader();

  void bind() const;
  void unbind() const;

  void upload_uniform_float4(const std::string& name, const glm::vec4&) const;
  void upload_uniform_mat4(const std::string& name, const glm::mat4&) const;

private:
  uint32_t renderer_id_;
};

} // namespace hazel
