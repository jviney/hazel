#pragma once

namespace hazel
{

class VertexBuffer
{
public:
  virtual ~VertexBuffer() {}

  virtual void bind() const = 0;
  virtual void unbind() const = 0;

  static std::unique_ptr<VertexBuffer> create(float* vertices, uint32_t size);
};

class IndexBuffer
{
public:
  virtual ~IndexBuffer() {}

  virtual void bind() const = 0;
  virtual void unbind() const = 0;

  virtual uint32_t count() const = 0;

  static std::unique_ptr<IndexBuffer> create(uint32_t* vertices, uint32_t size);
};

} // namespace hazel
