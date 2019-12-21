#pragma once

#include "hazel/events/event.hpp"

namespace hazel
{

class HZ_API KeyEvent : public Event
{
public:
  inline int key_code() const { return key_code_; }

  EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

protected:
  KeyEvent(int key_code) : key_code_(key_code) {}

  int key_code_ = 0;
};

class HZ_API KeyPressedEvent : public KeyEvent
{
public:
  KeyPressedEvent(int key_code, int repeat_count)
      : KeyEvent(key_code), repeat_count_(repeat_count) {}

  inline int repeat_count() const { return repeat_count_; }

  std::string to_string() const override {
    auto ss = std::stringstream();
    ss << "KeyPressedEvent: " << key_code_ << " (" << repeat_count_ << " repeats)";
    return ss.str();
  }

  EVENT_CLASS_TYPE(KeyPressed)

protected:
  int repeat_count_ = 0;
};

class HZ_API KeyReleasedEvent : public KeyEvent
{
public:
  KeyReleasedEvent(int key_code) : KeyEvent(key_code) {}

  std::string to_string() const override {
    auto ss = std::stringstream();
    ss << "KeyReleasedEvent: " << key_code_;
    return ss.str();
  }

  EVENT_CLASS_TYPE(KeyReleased)
};

class HZ_API KeyTypedEvent : public KeyEvent
{
public:
  KeyTypedEvent(int key_code) : KeyEvent(key_code) {}

  std::string to_string() const override {
    auto ss = std::stringstream();
    ss << "KeyTypedEvent: " << key_code_;
    return ss.str();
  }

  EVENT_CLASS_TYPE(KeyTyped)
};

} // namespace hazel
