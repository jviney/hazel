#pragma once

#include "hazel/core.hpp"

namespace hazel
{

// Events in Hazel are currently blocking, meaning when an event occurs it
// immediately gets dispatched and must be dealt with right then and there.
// For the future, a better strategy might be to buffer events in an event
// bus and process them during the "event" part of the update stage.

enum class EventType {
  None = 0,
  WindowClose,
  WindowResize,
  WindowFocus,
  WindowLostFocus,
  WindowMoved,
  AppTick,
  AppUpdate,
  AppRender,
  KeyPressed,
  KeyReleased,
  KeyTyped,
  MouseButtonPressed,
  MouseButtonReleased,
  MouseMoved,
  MouseScrolled
};

enum EventCategory {
  None = 0,
  EventCategoryApplication = BIT(0),
  EventCategoryInput = BIT(1),
  EventCategoryKeyboard = BIT(2),
  EventCategoryMouse = BIT(3),
  EventCategoryMouseButton = BIT(4)
};

#define EVENT_CLASS_TYPE(type)                                            \
  static EventType static_type() { return EventType::type; }              \
  virtual EventType event_type() const override { return static_type(); } \
  virtual const char* name() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) \
  virtual int category_flags() const override { return category; }

class Event
{
  friend class EventDispatcher;

public:
  virtual EventType event_type() const = 0;
  virtual const char* name() const = 0;
  virtual int category_flags() const = 0;
  virtual std::string to_string() const { return name(); }

  inline bool is_in_category(EventCategory category) { return category_flags() & category; }
  inline bool handled() const { return handled_; }

protected:
  bool handled_ = false;
};

class EventDispatcher
{
public:
  EventDispatcher(Event& event) : event_(event) {}

  template <typename T, typename F>
  bool dispatch(const F& func) {
    if (event_.event_type() == T::static_type()) {
      event_.handled_ = func(*(T*) &event_);
      return true;
    }

    return false;
  }

private:
  Event& event_;
};

inline std::ostream& operator<<(std::ostream& os, const Event& e) { return os << e.to_string(); }

} // namespace hazel
