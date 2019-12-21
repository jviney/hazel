#pragma once

#include "hazel/input.hpp"

namespace hazel
{

class WindowsInput final : public Input
{
protected:
  bool is_key_pressed_impl(int key_code) const override;

  bool is_mouse_button_pressed_impl(int button) const override;
  std::pair<float, float> mouse_position_impl() const override;
  float mouse_x_impl() const override;
  float mouse_y_impl() const override;
};

} // namespace hazel
