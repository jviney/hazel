#pragma once

#include "hazel/renderer/render_command.hpp"
#include "hazel/renderer/renderer_api.hpp"
#include "hazel/renderer/vertex_array.hpp"

namespace hazel
{

class Renderer
{
public:
  static void begin_scene();
  static void end_scene();

  static void submit(VertexArray*);

  static inline RendererAPI::API api() { return RendererAPI::api(); }
};

} // namespace hazel
