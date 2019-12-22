#pragma once

namespace hazel
{

class Shader
{
public:
  Shader(const std::string& vertex_source, const std::string& fragment_source);
  ~Shader();

  void bind() const;
  void unbind() const;

private:
  uint32_t renderer_id_;
};

} // namespace hazel
