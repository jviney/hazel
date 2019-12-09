#include <functional>
#include <memory>
#include <string>
#include <vector>

#ifdef _MSC_VER
  #ifdef HZ_SHARED_LIBRARY
    #define HZ_API __declspec(dllexport)
  #else
    #define HZ_API __declspec(dllimport)
  #endif
#else
  #ifdef HZ_SHARED_LIBRARY
    #define HZ_API __attribute__((visibility("default")))
  #else
    #define HZ_API
  #endif
#endif

#define BIT(x) (1 << x)
