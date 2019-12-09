#pragma once

#include "hazel/events/event.hpp"

#include <sstream>

namespace hazel
{

class HZ_API MouseMovedEvent : public Event
{
public:
  MouseMovedEvent(float x, float y) : x_(x), y_(y) {}

  inline float x() const { return x_; }
  inline float y() const { return y_; }

  std::string to_string() const override {
    auto ss = std::stringstream();
    ss << "MouseMovedEvent: " << x_ << ", " << y_;
    return ss.str();
  }

  EVENT_CLASS_TYPE(MouseMoved)
  EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

private:
  float x_{};
  float y_{};
};

class HZ_API MouseScrolledEvent : public Event
{
public:
  MouseScrolledEvent(float x_offset, float y_offset) : x_offset_(x_offset), y_offset_(y_offset) {}

  inline float x_offset() const { return x_offset_; }
  inline float y_offset() const { return y_offset_; }

  std::string to_string() const override {
    auto ss = std::stringstream();
    ss << "MouseScrolledEvent: " << x_offset_ << ", " << y_offset_;
    return ss.str();
  }

  EVENT_CLASS_TYPE(MouseScrolled)
  EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

private:
  float x_offset_{};
  float y_offset_{};
};

class HZ_API MouseButtonEvent : public Event
{
public:
  inline int mouse_button() const { return button_; }

  EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

protected:
  MouseButtonEvent(int button) : button_(button) {}

  int button_{};
};

class HZ_API MouseButtonPressedEvent : public MouseButtonEvent
{
public:
  MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}

  std::string to_string() const override {
    auto ss = std::stringstream();
    ss << "MouseButtonPressedEvent: " << button_;
    return ss.str();
  }

  EVENT_CLASS_TYPE(MouseButtonPressed)
};

class HZ_API MouseButtonReleasedEvent : public MouseButtonEvent
{
public:
  MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}

  std::string to_string() const override {
    auto ss = std::stringstream();
    ss << "MouseButtonReleasedEvent: " << button_;
    return ss.str();
  }

  EVENT_CLASS_TYPE(MouseButtonReleased)
};

} // namespace hazel
