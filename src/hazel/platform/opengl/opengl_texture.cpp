#include "hazel/core.hpp"
#include "hazel/log.hpp"
#include "hazel/platform/opengl/opengl_texture.hpp"

#include <glad/glad.h>
#include <stb_image.h>

namespace hazel
{

OpenGLTexture2D::OpenGLTexture2D(const std::string& path) : path_(path) {
  stbi_set_flip_vertically_on_load(1);

  int width, height, channels;
  auto* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
  HZ_CORE_ASSERT(data, "failed to load image");

  width_ = width;
  height_ = height;

  glGenTextures(1, &renderer_id_);
  glBindTexture(GL_TEXTURE_2D, renderer_id_);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width_, height_, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

  glBindTexture(GL_TEXTURE_2D, 0);

  stbi_image_free(data);
}

OpenGLTexture2D::~OpenGLTexture2D() {}

void OpenGLTexture2D::bind(uint32_t slot) const {
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, renderer_id_);
}

} // namespace hazel
