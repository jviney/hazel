#pragma once

#include "hazel/core.hpp"

namespace hazel
{

class Shader
{
public:
  virtual ~Shader() = default;

  virtual void bind() const = 0;
  virtual void unbind() const = 0;

  static Scope<Shader> create(const std::string& filepath);
  static Scope<Shader> create(const std::string& vertex_source, const std::string& fragment_source);
};

} // namespace hazel
