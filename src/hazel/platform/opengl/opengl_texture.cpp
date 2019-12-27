#include "hazel/core/core.hpp"
#include "hazel/platform/opengl/opengl_texture.hpp"

#include <glad/glad.h>
#include <stb_image.h>

namespace hazel
{

OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height) : width_(width), height_(height) {
  internal_format_ = GL_RGBA8;
  data_format_ = GL_RGBA;

  glGenTextures(1, &renderer_id_);
  glBindTexture(GL_TEXTURE_2D, renderer_id_);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

OpenGLTexture2D::OpenGLTexture2D(const std::string& path) : path_(path) {
  stbi_set_flip_vertically_on_load(1);

  int width, height, channels;
  auto* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
  HZ_CORE_ASSERT(data, "failed to load image");

  width_ = width;
  height_ = height;

  if (channels == 4) {
    internal_format_ = GL_RGBA8;
    data_format_ = GL_RGBA;
  } else if (channels == 3) {
    internal_format_ = GL_RGB8;
    data_format_ = GL_RGB;
  }

  HZ_CORE_ASSERT(internal_format_ & data_format_, "format not supported");

  glGenTextures(1, &renderer_id_);
  glBindTexture(GL_TEXTURE_2D, renderer_id_);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexImage2D(GL_TEXTURE_2D, 0, internal_format_, width_, height_, 0, data_format_,
               GL_UNSIGNED_BYTE, data);

  glBindTexture(GL_TEXTURE_2D, 0);

  stbi_image_free(data);
}

OpenGLTexture2D::~OpenGLTexture2D() {}

void OpenGLTexture2D::set_data(void* data, size_t size) const {
  uint32_t bpp = data_format_ == GL_RGBA ? 4 : 3;
  HZ_CORE_ASSERT(size == width_ * height_ * bpp, "data must be entire texture");

  glBindTexture(GL_TEXTURE_2D, renderer_id_);

  glTexImage2D(GL_TEXTURE_2D, 0, internal_format_, width_, height_, 0, data_format_,
               GL_UNSIGNED_BYTE, data);
};

void OpenGLTexture2D::bind(uint32_t slot) const {
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, renderer_id_);
}

} // namespace hazel
