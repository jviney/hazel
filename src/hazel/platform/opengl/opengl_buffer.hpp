#pragma once

#include "hazel/renderer/buffer.hpp"

namespace hazel
{

class OpenGLVertexBuffer : public VertexBuffer
{
public:
  OpenGLVertexBuffer(float* vertices, uint32_t size);
  ~OpenGLVertexBuffer();

  void bind() const override;
  void unbind() const override;

  const BufferLayout& layout() const override { return layout_; }
  void set_layout(const BufferLayout& layout) override { layout_ = layout; };

private:
  uint32_t renderer_id_;
  BufferLayout layout_;
};

class OpenGLIndexBuffer : public IndexBuffer
{
public:
  OpenGLIndexBuffer(uint32_t* vertices, uint32_t count);
  ~OpenGLIndexBuffer();

  void bind() const override;
  void unbind() const override;

  uint32_t count() const override { return count_; }

private:
  uint32_t renderer_id_;
  uint32_t count_;
};

} // namespace hazel
