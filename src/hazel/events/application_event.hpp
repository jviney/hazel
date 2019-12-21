#pragma once

#include "hazel/events/event.hpp"

namespace hazel
{

class WindowResizeEvent : public Event
{
public:
  WindowResizeEvent(unsigned int width, unsigned int height) : width_(width), height_(height) {}

  inline unsigned int width() const { return width_; }
  inline unsigned int height() const { return height_; }

  std::string to_string() const override {
    auto ss = std::stringstream();
    ss << "WindowResizeEvent: " << width_ << ", " << height_;
    return ss.str();
  }

  EVENT_CLASS_TYPE(WindowResize)
  EVENT_CLASS_CATEGORY(EventCategoryApplication)
private:
  unsigned int width_ = 0, height_ = 0;
};

class WindowCloseEvent : public Event
{
public:
  WindowCloseEvent() {}

  EVENT_CLASS_TYPE(WindowClose)
  EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class AppTickEvent : public Event
{
  AppTickEvent() {}

  EVENT_CLASS_TYPE(AppTick)
  EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class AppUpdateEvent : public Event
{
public:
  AppUpdateEvent() {}

  EVENT_CLASS_TYPE(AppUpdate)
  EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class AppRenderEvent : public Event
{
public:
  AppRenderEvent() {}

  EVENT_CLASS_TYPE(AppRender)
  EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

} // namespace hazel
