#pragma once

#include "hazel/core/core.hpp"

#include <glm/glm.hpp>

namespace hazel
{

class Shader
{
public:
  virtual ~Shader() = default;

  virtual void bind() const = 0;
  virtual void unbind() const = 0;

  virtual const std::string& name() const = 0;

  virtual void set_float3(const std::string& name, const glm::vec3&) const = 0;
  virtual void set_float4(const std::string& name, const glm::vec4&) const = 0;
  virtual void set_mat4(const std::string& name, const glm::mat4&) const = 0;

  static Scope<Shader> create(const std::string& filepath);
  static Scope<Shader> create(const std::string& name, const std::string& vertex_source,
                              const std::string& fragment_source);
};

class ShaderLibrary
{
public:
  void add(Ref<Shader>);
  void add(const std::string& name, Ref<Shader>);

  Ref<Shader> load(const std::string& filepath);
  Ref<Shader> load(const std::string& name, const std::string& filepath);

  Ref<Shader> get(const std::string& name);

  bool exists(const std::string& name) const;

private:
  std::unordered_map<std::string, Ref<Shader>> shaders_;
};

} // namespace hazel
