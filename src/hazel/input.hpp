#pragma once

#include "hazel/core.hpp"

namespace hazel
{

class HZ_API Input
{
public:
  virtual ~Input() {}

  static bool is_key_pressed(int key_code) { return instance_->is_key_pressed_impl(key_code); }

  static bool is_mouse_button_pressed(int button) {
    return instance_->is_mouse_button_pressed_impl(button);
  }

  static std::pair<float, float> mouse_position() { return instance_->mouse_position_impl(); }

  static float mouse_x() { return instance_->mouse_x_impl(); }
  static float mouse_y() { return instance_->mouse_y_impl(); }

protected:
  virtual bool is_key_pressed_impl(int key_code) const = 0;
  virtual bool is_mouse_button_pressed_impl(int key_code) const = 0;
  virtual std::pair<float, float> mouse_position_impl() const = 0;
  virtual float mouse_x_impl() const = 0;
  virtual float mouse_y_impl() const = 0;

private:
  static std::unique_ptr<Input> instance_;
};

} // namespace hazel
