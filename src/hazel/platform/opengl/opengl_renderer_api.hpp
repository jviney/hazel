#pragma once

#include "hazel/renderer/renderer_api.hpp"

namespace hazel
{

class OpenGLRendererAPI : public RendererAPI
{
public:
  void init() override;
  void set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
  void clear() override;
  void set_clear_color(const glm::vec4&) override;

  void draw_indexed(const VertexArray*) override;
};

} // namespace hazel
