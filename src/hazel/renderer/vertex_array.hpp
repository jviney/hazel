#pragma once

#include "hazel/renderer/buffer.hpp"

namespace hazel
{

class VertexArray
{
public:
  virtual ~VertexArray() {}

  virtual void bind() const = 0;
  virtual void unbind() const = 0;

  virtual void add_vertex_buffer(std::shared_ptr<VertexBuffer>) = 0;
  virtual void set_index_buffer(std::shared_ptr<IndexBuffer>) = 0;

  virtual const std::vector<std::shared_ptr<VertexBuffer>>& vertex_buffers() const = 0;
  virtual const std::shared_ptr<IndexBuffer>& index_buffer() const = 0;

  static std::unique_ptr<VertexArray> create();
};

} // namespace hazel
