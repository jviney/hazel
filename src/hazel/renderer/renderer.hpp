#pragma once

namespace hazel
{

enum class RendererAPI { None = 0, OpenGL = 1 };

class Renderer
{
public:
  static inline RendererAPI api() { return renderer_api_; }

private:
  static RendererAPI renderer_api_;
};

} // namespace hazel
