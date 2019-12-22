#pragma once

#include <glm/glm.hpp>

#include "hazel/renderer/vertex_array.hpp"

namespace hazel
{

class RendererAPI
{
public:
  enum class API { None = 0, OpenGL = 1 };

  virtual void clear() = 0;
  virtual void set_clear_color(const glm::vec4&) = 0;

  virtual void draw_indexed(const VertexArray*) = 0;

  static API api() { return api_; }

private:
  static API api_;
};

} // namespace hazel
