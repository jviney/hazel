#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <utility>

#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
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
