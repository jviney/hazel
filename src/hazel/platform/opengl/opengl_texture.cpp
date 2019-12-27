#include "hazel/core/core.hpp"
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

  GLenum internal_format = 0, data_format = 0;

  if (channels == 4) {
    internal_format = GL_RGBA8;
    data_format = GL_RGBA;
  } else if (channels == 3) {
    internal_format = GL_RGB8;
    data_format = GL_RGB;
  }

  HZ_CORE_ASSERT(internal_format & data_format, "format not supported");

  glGenTextures(1, &renderer_id_);
  glBindTexture(GL_TEXTURE_2D, renderer_id_);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width_, height_, 0, data_format, GL_UNSIGNED_BYTE,
               data);

  glBindTexture(GL_TEXTURE_2D, 0);

  stbi_image_free(data);
}

OpenGLTexture2D::~OpenGLTexture2D() {}

void OpenGLTexture2D::bind(uint32_t slot) const {
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, renderer_id_);
}

} // namespace hazel
