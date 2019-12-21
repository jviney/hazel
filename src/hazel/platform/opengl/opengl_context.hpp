#pragma once

#include "hazel/renderer/graphics_context.hpp"

class GLFWwindow;

namespace hazel
{

class OpenGLContext : public GraphicsContext
{
public:
  OpenGLContext(GLFWwindow* window);
  ~OpenGLContext() {}

  void init() override;
  void swap_buffers() override;

private:
  GLFWwindow* window_;
};

} // namespace hazel
