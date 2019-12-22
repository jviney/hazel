#pragma once

#include "hazel/renderer/renderer_api.hpp"

namespace hazel
{

class OpenGLRendererAPI : public RendererAPI
{
public:
  void clear() override;
  void set_clear_color(const glm::vec4&) override;

  void draw_indexed(const VertexArray*) override;
};

} // namespace hazel
