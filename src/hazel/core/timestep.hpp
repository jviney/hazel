#pragma once

namespace hazel
{

class Timestep
{
public:
  Timestep(float time = 0.0f) : time_(time) {}

  float seconds() const { return time_; }
  float milliseconds() const { return time_ * 1000.0f; }

  operator float() const { return time_; }

private:
  float time_ = 0.0f;
};

} // namespace hazel
