#pragma once

#include "hazel/renderer/renderer_api.hpp"
#include "hazel/renderer/vertex_array.hpp"

namespace hazel
{

class RenderCommand
{
public:
  inline static void init() { renderer_api_->init(); }

  inline static void set_clear_color(const glm::vec4& color) {
    renderer_api_->set_clear_color(color);
  }

  inline static void clear() { renderer_api_->clear(); }

  inline static void draw_indexed(const VertexArray* vertex_array) {
    renderer_api_->draw_indexed(vertex_array);
  }

private:
  static RendererAPI* renderer_api_;
};

} // namespace hazel
