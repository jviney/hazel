#pragma once

namespace hazel
{

class GraphicsContext
{
public:
  virtual ~GraphicsContext() {}

  virtual void init() = 0;
  virtual void swap_buffers() = 0;
};

} // namespace hazel
