#pragma once

#include "hazel/renderer/texture.hpp"

namespace hazel
{

class OpenGLTexture2D : public Texture2D
{
public:
  OpenGLTexture2D(const std::string& path);
  ~OpenGLTexture2D();

  uint32_t width() const override { return width_; }
  uint32_t height() const override { return height_; }

  void bind(uint32_t slot = 0) const override;

private:
  std::string path_;
  uint32_t width_{};
  uint32_t height_{};
  uint32_t renderer_id_{};
};

} // namespace hazel
