#pragma once

#include "hazel/core/core.hpp"

namespace hazel
{

class Texture
{
public:
  virtual ~Texture() = default;

  virtual uint32_t width() const = 0;
  virtual uint32_t height() const = 0;

  virtual void bind(uint32_t slot = 0) const = 0;
};

class Texture2D : public Texture
{
public:
  ~Texture2D() = default;

  static Scope<Texture2D> create(const std::string& path);
};

} // namespace hazel
