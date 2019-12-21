#include "hazel/core.hpp"
#include "hazel/platform/opengl/opengl_context.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace hazel
{

OpenGLContext::OpenGLContext(GLFWwindow* window) : window_(window) {
  HZ_CORE_ASSERT(window_, "window handle is null");
}

void OpenGLContext::init() {
  glfwMakeContextCurrent(window_);

  int status = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
  HZ_CORE_ASSERT(status, "failed to initialize glad");
}

void OpenGLContext::swap_buffers() { glfwSwapBuffers(window_); }

} // namespace hazel
