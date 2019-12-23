#pragma once

#include "hazel/core.hpp"
#include "hazel/renderer/buffer.hpp"

namespace hazel
{

class VertexArray
{
public:
  virtual ~VertexArray() {}

  virtual void bind() const = 0;
  virtual void unbind() const = 0;

  virtual void add_vertex_buffer(Ref<VertexBuffer>) = 0;
  virtual void set_index_buffer(Ref<IndexBuffer>) = 0;

  virtual const std::vector<Ref<VertexBuffer>>& vertex_buffers() const = 0;
  virtual const Ref<IndexBuffer>& index_buffer() const = 0;

  static Scope<VertexArray> create();
};

} // namespace hazel
